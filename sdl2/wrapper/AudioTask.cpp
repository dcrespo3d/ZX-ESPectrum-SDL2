#include "AudioTask.h"
#include "wrap_pwm_audio.h"
#include <stdio.h>
#include <deque>
#include <memory.h>

static SDL_AudioSpec want, have;
static SDL_AudioDeviceID dev;

static int prevSampleRate = -1;
static int sampleRate = -1;
static int bufferSize = 0;

typedef std::deque<uint8_t*> AudioBufQueue;
static AudioBufQueue queue;
static AudioBufQueue discardableQueue;

//#define USE_QUEUE

#ifndef USE_QUEUE
static uint8_t buf0[1024]; // must be equal or greater than ESP_AUDIO_SAMPLES at ESPectrum.h
static uint8_t buf1[1024];
static bool altbuf = false;
static int urcount = 0;
#endif


static void clearQueue(AudioBufQueue& q) {
	while (!q.empty()) {
		SDL_LockAudioDevice(dev);
		uint8_t* buf = q.front();
		q.pop_front();
		SDL_UnlockAudioDevice(dev);
		delete[] buf;
	}
}

static void initDevice();
static void closeDevice();
static void audioCallback(void* userdata, Uint8* stream, int len);

esp_err_t pwm_audio_set_param(int rate, ledc_timer_bit_t bits, int ch)
{
	// heuristic
	if (rate == 31200)
		rate = 31232;

	if (rate == 31630)
		rate = 31642;

	sampleRate = rate;

	return 0;
}

esp_err_t pwm_audio_write(uint8_t* inbuf, size_t len, size_t* bytes_written, TickType_t ticks_to_wait)
{
	if (0 == inbuf) return 0;

#ifdef USE_QUEUE
	// avoid audio lag by discarding excess buffers (will break audio at some points)
	if (queue.size() > 1) {
		printf("discarding audio buffer\n");
		return 0;
	}

	uint8_t* bufcopy = new uint8_t[len];
	if (0 == bufcopy) return 0;

	memcpy(bufcopy, inbuf, len);

	SDL_LockAudioDevice(dev);
	queue.push_back(bufcopy);
	SDL_UnlockAudioDevice(dev);
#else
	if (!altbuf)
		memcpy(buf0, inbuf, len);
	else
		memcpy(buf1, inbuf, len);
	altbuf = !altbuf;
	urcount = 0;
#endif

	bufferSize = len;

	return 0;
}


static void audioCallback(void* userdata, Uint8* stream, int len)
{
	if (0 == stream) return;

//#define FORCE_SAWTOOTH
#ifdef FORCE_SAWTOOTH
	static int v = 0;
	Uint8* ptr = stream;
	for (int i = 0; i < len; i++) {
		*ptr++ = v & 0xFF;
		v++;
	}
	return;
#endif

#ifdef USE_QUEUE
	if (queue.empty()) {
		printf("audio buffer underrun\n");
		memset(stream, 127, len);
		return;
	}
	//printf("queue size: %d\n", queue.size());


	uint8_t* buf = queue.front();
	queue.pop_front();
	memcpy(stream, buf, len);
	discardableQueue.push_back(buf);
#else
	if (urcount > 2) {
		memset(stream, 127, len);
		return;
	}
	if (altbuf)
		memcpy(stream, buf0, len);
	else
		memcpy(stream, buf1, len);
	urcount++;
#endif


}

static void initDevice()
{
	SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
	want.freq = sampleRate;
	want.format = AUDIO_U8;
	want.channels = 1;
	want.samples = bufferSize;
	want.callback = audioCallback;  // you wrote this function elsewhere.
	dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
	printf("opened audio device: %d\n", dev);
	SDL_PauseAudioDevice(dev, 0);
}

static void closeDevice()
{
	SDL_LockAudioDevice(dev);
	SDL_CloseAudioDevice(dev);

}



void AudioTask::init()
{
}

void AudioTask::update()
{
	clearQueue(discardableQueue);

	if (bufferSize == 0) return;

	if (prevSampleRate != sampleRate) {
		printf("sample rate changed: %d\n", sampleRate);
		closeDevice();
		initDevice();
	}

	prevSampleRate = sampleRate;

}


