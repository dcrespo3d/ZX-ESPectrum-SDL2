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

static std::deque<uint8_t*> queue;

static void initDevice();
static void closeDevice();
static void audioCallback(void* userdata, Uint8* stream, int len);

esp_err_t pwm_audio_set_param(int rate, ledc_timer_bit_t bits, int ch)
{
	sampleRate = rate;

	return 0;
}

esp_err_t pwm_audio_write(uint8_t* inbuf, size_t len, size_t* bytes_written, TickType_t ticks_to_wait)
{
	if (0 == inbuf) return 0;

	if (queue.size() > 1) return 0;

	uint8_t* bufcopy = new uint8_t[len];
	if (0 == bufcopy) return 0;

	memcpy(bufcopy, inbuf, len);

	queue.push_back(bufcopy);

	bufferSize = len;

	return 0;
}


static void audioCallback(void* userdata, Uint8* stream, int len)
{
	if (0 == stream) return;
	if (queue.empty()) {
		memset(stream, 127, len);
		return;
	}

	uint8_t* buf = queue.front();
	queue.pop_front();
	memcpy(stream, buf, len);
	delete[] buf;

	printf("queue size: %d\n", queue.size());


	// sawtooth
	//static int v = 0;
	//Uint8* ptr = stream;
	//for (int i = 0; i < len; i++) {
	//	*ptr++ = v & 0xFF;
	//	v++;
	//}
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

	while (!queue.empty()) {
		uint8_t* buf = queue.front();
		queue.pop_front();
		delete[] buf;
	}
}



void AudioTask::init()
{
}

void AudioTask::update()
{
	if (bufferSize == 0) return;

	if (prevSampleRate != sampleRate) {
		printf("sample rate changed: %d\n", sampleRate);
		closeDevice();
		initDevice();
	}

	prevSampleRate = sampleRate;

}


