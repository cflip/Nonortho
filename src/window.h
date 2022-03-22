#pragma once

#include <functional>

class Bitmap;

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Window {
public:
	Window(const char* title, int width, int height, int scale);
	~Window();

	void update();
	void draw(Bitmap&);

	bool shouldClose() const { return !m_isRunning; }

	void onMouseDown(std::function<void(int, int, int)> callback) { m_mouseDown = callback; }
	void onMouseUp(std::function<void(int, int, int)> callback) { m_mouseUp = callback; }
	void onMouseMove(std::function<void(int, int)> callback) { m_mouseMove = callback; }
	void onKeyDown(std::function<void(int)> callback) { m_keyDown = callback; }
	void onKeyUp(std::function<void(int)> callback) { m_keyUp = callback; }

private:
	bool m_isRunning;

	int m_width, m_height, m_scale;

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;

	std::function<void(int, int, int)> m_mouseDown;
	std::function<void(int, int, int)> m_mouseUp;
	std::function<void(int, int)> m_mouseMove;
	std::function<void(int)> m_keyDown;
	std::function<void(int)> m_keyUp;
};