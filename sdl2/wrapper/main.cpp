#include <stdio.h>

//#ifdef ESP32_SDL2_WRAPPER
//#error "USING wrapper"
//#else
//#error "NOT using wrapper"
//#endif

#include "ESPectrum.h"
#include "Video.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

#include "SDL.h"

extern void initKeyEventQueue();
extern void pushToKeyEventQueue(uint32_t type, SDL_Scancode ekey);

bool isLittleEndian()
{
	uint16_t val16 = 0x1;
	uint8_t* ptr8 = (uint8_t*)(&val16);
	return (*ptr8 == 1);
}

int espectrum_proc(void* data)
{
	ESPectrum::setup();
	ESPectrum::loop();
	return 0;
}

int main(int argc, char* argv[])
{
	SDL_CreateThread(espectrum_proc, "espectrum", NULL);

	if (isLittleEndian()) std::cout << "Running on little endian machine" << std::endl;
	else                  std::cout << "Running on big endian machine" << std::endl;

	SDL_Init(0);

	std::cout << "Testing video drivers..." << '\n';
	std::vector< bool > drivers(SDL_GetNumVideoDrivers());
	for (int i = 0; i < drivers.size(); ++i)
	{
		drivers[i] = (0 == SDL_VideoInit(SDL_GetVideoDriver(i)));
		SDL_VideoQuit();
	}

	std::cout << "SDL_VIDEODRIVER available:";
	for (int i = 0; i < drivers.size(); ++i)
	{
		std::cout << " " << SDL_GetVideoDriver(i);
	}
	std::cout << '\n';

	std::cout << "SDL_VIDEODRIVER usable   :";
	for (int i = 0; i < drivers.size(); ++i)
	{
		if (!drivers[i]) continue;
		std::cout << " " << SDL_GetVideoDriver(i);
	}
	std::cout << '\n';

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Error: %s\n", SDL_GetError());
		return 0;
	}

	SDL_Window* window = 0;

	bool FULLSCREEN = false;
	if (FULLSCREEN)
	{
		unsigned int sdl_flags = SDL_WINDOW_FULLSCREEN;
		SDL_DisplayMode current;
		current.w = 800;
		current.h = 600;
		//getting current display-resolution
		if (SDL_GetDesktopDisplayMode(0, &current) != 0) {
			printf("Could not retrieve current display resolution: %s\n", SDL_GetError());
			SDL_Quit();
			return -1;
		}
		printf("Setting resolution to %dx%d\n", current.w, current.h);

		window = SDL_CreateWindow
		(
			"ZX-ESPectrum-SDL2",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			current.w, current.h,
			SDL_WINDOW_FULLSCREEN
		);
	}
	else
	{
		window = SDL_CreateWindow
		(
			"ZX-ESPectrum-SDL2",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1080, 600,
			SDL_WINDOW_SHOWN
		);
	}

	if (window) std::cout << "Window has been created" << std::endl;
	else { std::cout << "Window has NOT been created" << std::endl; SDL_Quit(); return -1; }

	unsigned vsyncFlag = SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* renderer = SDL_CreateRenderer
	(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | vsyncFlag
	);

	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);
	std::cout << "Renderer name: " << info.name << std::endl;
	std::cout << "Texture formats: " << std::endl;
	for (Uint32 i = 0; i < info.num_texture_formats; i++)
	{
		std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
	}

	const unsigned int texWidth = 360;
	const unsigned int texHeight = 200;
	SDL_Texture* texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		texWidth, texHeight
	);

	std::vector< unsigned char > pixels(texWidth * texHeight * 4, 0);

	SDL_Event event;
	bool running = true;
	bool useLocktexture = false;

	unsigned int frames = 0;
	Uint64 start = SDL_GetPerformanceCounter();

	initKeyEventQueue();

	while (running)
	{

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type)
			{
				running = false;
				break;
			}

			if (SDL_KEYDOWN == event.type || SDL_KEYUP == event.type)
			{
				pushToKeyEventQueue(event.type, event.key.keysym.scancode);
			}


			//if ((SDL_QUIT == event.type) ||
			//	(SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
			//{
			//	running = false;
			//	break;
			//}
			//if (SDL_KEYDOWN == event.type && SDL_SCANCODE_L == event.key.keysym.scancode)
			//{
			//	useLocktexture = !useLocktexture;
			//	std::cout << "Using " << (useLocktexture ? "SDL_LockTexture() + memcpy()" : "SDL_UpdateTexture()") << std::endl;
			//}
		}

		// splat down some random pixels
		//for (unsigned int i = 0; i < 1000; i++)
		//{
		//	const unsigned int x = rand() % texWidth;
		//	const unsigned int y = rand() % texHeight;

		//	const unsigned int offset = (texWidth * 4 * y) + x * 4;
		//	pixels[offset + 0] = rand() % 256;        // b
		//	pixels[offset + 1] = rand() % 256;        // g
		//	pixels[offset + 2] = rand() % 256;        // r
		//	pixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a
		//}

		//SDL_PumpEvents();

		//const uint8_t* sdlkeys = SDL_GetKeyboardState(NULL);
		//if (sdlkeys[SDL_SCANCODE_RETURN]) printf("return pressed\n");

		// framebuffer dump
		uint8_t* dst = &pixels[0];
		uint8_t ctable[] = { 0, 128, 192, 255 };
		int dx = VIDEO::vga.mode.hRes;
		int dy = VIDEO::vga.mode.vRes / VIDEO::vga.mode.vDiv;
		int dx4 = dx >> 2;
		for (int y = 0; y < dy; y++)
		{
			const uint8_t* src = VIDEO::vga.backBuffer[y];
			uint8_t v;
			for (int x = 0; x < dx; x += 4)
			{
				v = src[2];
				*dst++ = ctable[(v & 0x03)     ]; // b
				*dst++ = ctable[(v & 0x0C) >> 2]; // g
				*dst++ = ctable[(v & 0x30) >> 4]; // r
				*dst++ = SDL_ALPHA_OPAQUE;        // a
				v = src[3];
				*dst++ = ctable[(v & 0x03)     ]; // b
				*dst++ = ctable[(v & 0x0C) >> 2]; // g
				*dst++ = ctable[(v & 0x30) >> 4]; // r
				*dst++ = SDL_ALPHA_OPAQUE;        // a
				v = src[0];
				*dst++ = ctable[(v & 0x03)     ]; // b
				*dst++ = ctable[(v & 0x0C) >> 2]; // g
				*dst++ = ctable[(v & 0x30) >> 4]; // r
				*dst++ = SDL_ALPHA_OPAQUE;        // a
				v = src[1];
				*dst++ = ctable[(v & 0x03)     ]; // b
				*dst++ = ctable[(v & 0x0C) >> 2]; // g
				*dst++ = ctable[(v & 0x30) >> 4]; // r
				*dst++ = SDL_ALPHA_OPAQUE;        // a
				src += 4;
			}
		}


		if (useLocktexture)
		{
			unsigned char* lockedPixels = nullptr;
			int pitch = 0;
			SDL_LockTexture
			(
				texture,
				NULL,
				reinterpret_cast<void**>(&lockedPixels),
				&pitch
			);
			std::memcpy(lockedPixels, pixels.data(), pixels.size());
			SDL_UnlockTexture(texture);
		}
		else
		{
			SDL_UpdateTexture
			(
				texture,
				NULL,
				pixels.data(),
				texWidth * 4
			);
		}

		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		continue;

		frames++;
		const Uint64 end = SDL_GetPerformanceCounter();
		const static Uint64 freq = SDL_GetPerformanceFrequency();
		const double seconds = (end - start) / static_cast<double>(freq);
		if (seconds > 2.0)
		{
			std::cout
				<< frames << " frames in "
				<< std::setprecision(1) << std::fixed << seconds << " seconds = "
				<< std::setprecision(1) << std::fixed << frames / seconds << " FPS ("
				<< std::setprecision(3) << std::fixed << (seconds * 1000.0) / frames << " ms/frame)"
				<< std::endl;
			start = end;
			frames = 0;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
