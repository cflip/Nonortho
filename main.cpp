#define SDL_MAIN_HANDLED
#include <cmath>

#include "window.h"
#include "windows.h"
#include "bitmap.h"
#include "level.h"

#include "iostream"
#pragma comment(lib, "winmm.lib") // For audio files


struct Point2D {
	float x, y;
};

Point2D screenToTile(Point2D source)
{
	Point2D result;

	constexpr int TILE_SIZE = 24;

	float mx = source.x;
	float my = source.y;
	float xx = (mx / TILE_SIZE + my / (TILE_SIZE / 2.f));
	float yy = (my / (TILE_SIZE / 2.f) - (mx / TILE_SIZE));

	result.x = floor(xx - 1.5f); // Magic numbers
	result.y = floor(yy - 0.5f);

	return result;
}

void DrawLevel(Bitmap& bitmap, Bitmap& tiles, Level& level, int xo, int yo)
{
	bitmap.clear(0xff224466);

	for (int y = 0; y < 32; ++y) {
		for (int x = 0; x < 32; ++x) {
			constexpr int TILE_SIZE = 24;

			auto tile = level.get(x, y);

			int xx = (x - y) * (TILE_SIZE / 2) - xo;
			int yy = (x + y) * (TILE_SIZE / 4) - yo;

			int tx = (x + y) % 2;
			int ty = 0;

			if (tile == NorthSouth) {
				tx = 0;
				ty = 2;
			}
			else if (tile == EastWest) {
				tx = 1;
				ty = 2;
			}
			else if (tile == SouthEast) {
				tx = 0;
				ty = 3;
			}
			else if (tile == SouthWest) {
				tx = 3;
				ty = 3;
			}
			else if (tile == NorthWest) {
				tx = 1;
				ty = 3;
			}
			else if (tile == NorthEast) {
				tx = 2;
				ty = 3;
			}

			bitmap.blit(tiles, xx, yy, tx * 24, ty * 24, TILE_SIZE, TILE_SIZE);
		}
	}
}

int main(int argc, char **argv)
{
	constexpr int Height = 240;
	constexpr int Width = Height * 16 / 9;
	constexpr int Scale = 3;

	Window window("Nonortho", Width, Height, Scale);

	Bitmap tiles("tiles.png");
	Level level(32, 32);

	Bitmap bitmap(Width, Height);

	//bool audio_play1 = PlaySound(TEXT("res\\Starliner.wav"), NULL, SND_LOOP | SND_ASYNC);
	//Adds Starliner.wav and plays it on startup and loops,
	//I imagine this would get very annoying during debug so it's commented out for now.

	int xOffs = 0, yOffs = 0;
	int xDrag, yDrag;
	bool isDragging = false;



	window.onMouseDown([&](int button, int x, int y) {
		if (button == 1) {
			static const auto update_direction = [&](int xt, int yt) {
				if (level.get(xt, yt) > 0)
					level.set(xt, yt, ChooseDirection(level, xt, yt));
			};

			float mx = x / Scale + xOffs;
			float my = y / Scale + yOffs;
			Point2D tilePos = screenToTile({ mx, my });

			if (level.inBounds(tilePos.x, tilePos.y)) {
				auto tile = level.get(tilePos.x, tilePos.y);

				if (tile > 0) tile = 0;
				else tile = ChooseDirection(level, tilePos.x, tilePos.y);

				level.set(tilePos.x, tilePos.y, tile);

				update_direction(tilePos.x - 1, tilePos.y);
				update_direction(tilePos.x + 1, tilePos.y);
				update_direction(tilePos.x, tilePos.y - 1);
				update_direction(tilePos.x, tilePos.y + 1);
			}
		} else if (button == 2 && !isDragging) {
			xDrag = x;
			yDrag = y;
			isDragging = true;
		}
	});

	window.onMouseUp([&](int button, int x, int y) {
		if (button == 2 && isDragging) 
			isDragging = false;
	});

	window.onMouseMove([&](int x, int y) {
		if (isDragging) {
			xOffs -= (x - xDrag) / Scale;
			yOffs -= (y - yDrag) / Scale;
			xDrag = x;
			yDrag = y;
		}
	});

	while (!window.shouldClose()) {
		window.update();
		DrawLevel(bitmap, tiles, level, xOffs, yOffs);
		window.draw(bitmap);
	}

	return 0;
}