#include "window.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cstdio>

#include "bitmap.h"

Window::Window(const char* title, int width, int height, int scale)
	: m_width(width), m_height(height), m_scale(scale)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	m_window = SDL_CreateWindow("Nonortho", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

	m_isRunning = true;
}

Window::~Window()
{
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::update()
{
	static SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (m_mouseDown)
				m_mouseDown(event.button.button, event.button.x, event.button.y);
			break;
		case SDL_MOUSEMOTION:
			if (m_mouseMove)
				m_mouseMove(event.button.x, event.button.y);
			break;
		case SDL_MOUSEBUTTONUP:
			if (m_mouseUp)
				m_mouseUp(event.button.button, event.button.x, event.button.y);
			break;
		case SDL_KEYDOWN:
			if (m_keyDown)
				m_keyDown(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			if (m_keyUp)
				m_keyUp(event.key.keysym.sym);
			break;
		}
	}

	SDL_UpdateWindowSurface(m_window);
	SDL_Delay(10);
}

void Window::draw(Bitmap& bitmap)
{
	SDL_RenderClear(m_renderer);
	SDL_UpdateTexture(m_texture, nullptr, bitmap.data, m_width * 4);
	SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
	SDL_RenderPresent(m_renderer);
}