/*
	Author: bitluni 2019
	License: 
	Creative Commons Attribution ShareAlike 4.0
	https://creativecommons.org/licenses/by-sa/4.0/
	
	For further details check out: 
		https://youtube.com/bitlunislab
		https://github.com/bitluni
		http://bitluni.net
*/
#pragma once
#include "../Tools/Log.h"
#include <stdlib.h>

#define	STAILQ_ENTRY(type)						\
struct {								\
	struct type *stqe_next;	/* next element */			\
}

typedef struct lldesc_s {
	volatile uint32_t size : 12,
		length : 12,
		offset : 5, /* h/w reserved 5bit, s/w use it as offset in buffer */
		sosf : 1, /* start of sub-frame */
		eof : 1, /* end of frame */
		owner : 1; /* hw or sw */
	volatile uint8_t* buf;       /* point to buffer data */
	union {
		volatile uint32_t empty;
		STAILQ_ENTRY(lldesc_s) qe;  /* pointing to the next desc */
	};
} lldesc_t;

class DMABufferDescriptor : protected lldesc_t
{
  public:
	static void *allocateBuffer(int bytes, bool clear = true, unsigned long clearValue = 0)
	{
		bytes = (bytes + 3) & 0xfffffffc;
		//void *b = heap_caps_malloc(bytes, MALLOC_CAP_DMA);
		void *b = malloc(bytes);
		if (!b)
			DEBUG_PRINTLN("Failed to alloc dma buffer");
		if (clear)
			for (int i = 0; i < bytes / 4; i++)
				((unsigned long *)b)[i] = clearValue;
		return b;
	}

	static void **allocateDMABufferArray(int count, int bytes, bool clear = true, unsigned long clearValue = 0)
	{
		void **arr = (void **)malloc(count * sizeof(void *));
		if(!arr)
			ERROR("Not enough DMA memory");
		for (int i = 0; i < count; i++)
		{
			arr[i] = DMABufferDescriptor::allocateBuffer(bytes, true, clearValue);
			if(!arr[i])
				ERROR("Not enough DMA memory");
		}
		return arr;
	}

	void setBuffer(void *buffer, int bytes)
	{
		length = bytes;
		size = length;
		buf = (uint8_t *)buffer;
	}

	void *buffer() const
	{
		return (void *)buf;
	}

	void init()
	{
		length = 0;
		size = 0;
		owner = 1;
		sosf = 0;
		buf = (uint8_t *)0;
		offset = 0;
		empty = 0;
		eof = 1;
		qe.stqe_next = 0;
	}

	static DMABufferDescriptor *allocateDescriptors(int count)
	{
		//DMABufferDescriptor* b = (DMABufferDescriptor*)heap_caps_malloc(sizeof(DMABufferDescriptor) * count, MALLOC_CAP_DMA);
		DMABufferDescriptor* b = (DMABufferDescriptor*)malloc(sizeof(DMABufferDescriptor) * count);
		if (!b)
			DEBUG_PRINTLN("Failed to alloc DMABufferDescriptors");
		for (int i = 0; i < count; i++)
			b[i].init();
		return b;
	}

	static DMABufferDescriptor *allocateDescriptor(int bytes, bool allocBuffer = true, bool clear = true, unsigned long clearValue = 0)
	{
		bytes = (bytes + 3) & 0xfffffffc;
		//DMABufferDescriptor* b = (DMABufferDescriptor*)heap_caps_malloc(sizeof(DMABufferDescriptor), MALLOC_CAP_DMA);
		DMABufferDescriptor* b = (DMABufferDescriptor*)malloc(sizeof(DMABufferDescriptor));
		if (!b)
			DEBUG_PRINTLN("Failed to alloc DMABufferDescriptor");
		b->init();
		if (allocateBuffer)
			b->setBuffer(allocateBuffer(bytes, clear, clearValue), bytes);
		return b;
	}

	void next(DMABufferDescriptor &next)
	{
		qe.stqe_next = &next;
	}

	int sampleCount() const
	{
		return length / 4;
	}

	void destroy()
	{
		if (buf)
		{
			free((void *)buf);
			buf = 0;
		}
		free(this);
	}
};