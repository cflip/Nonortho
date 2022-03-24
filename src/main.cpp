#include "bitmap.h"
#include "level.h"
#include "train.h"
#include "util.h"
#include "window.h"

#include <SDL_keycode.h>

int main(int argc, char** argv)
{
	constexpr int Height = 240;
	constexpr int Width = Height * 16 / 9;
	constexpr int Scale = 3;

	Window window("Nonortho", Width, Height, Scale);
	Bitmap tiles("res/tiles.png");

	Level level(32, 32, tiles);
	Bitmap bitmap(Width, Height);

	Train engine(level);
	Train wagon(level);

	engine.addVehicle(&wagon);

	level.addVehicle(engine);
	level.addVehicle(wagon);

	int xOffs = 0, yOffs = 0;
	int xDrag, yDrag;
	Point2D hoveredTile = { 0, 0 };
	bool isDragging = false;

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;

	const int cameraMoveSpeed = 3;

	window.onMouseDown([&](int button, int x, int y) {
		if (button == 1) {
			int mx = x / Scale + xOffs;
			int my = y / Scale + yOffs;

			Point2D tilePos = ScreenToTile({ mx, my });
			level.toggleTile(tilePos.x, tilePos.y);
		} else if (button == 2 && !isDragging) {
			xDrag = x;
			yDrag = y;
			isDragging = true;
		} else {
			int mx = x / Scale + xOffs;
			int my = y / Scale + yOffs;
			auto pos = ScreenToTile({ mx, my });
			engine.setPosition(pos.x, pos.y);
			engine.setSpeed(0.2f);
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
		int mx = x / Scale + xOffs;
		int my = y / Scale + yOffs;
		hoveredTile = ScreenToTile({ mx, my });
	});

	window.onKeyDown([&](int keycode) {
		switch (keycode) {
		case SDLK_UP:
			up = true;
			break;
		case SDLK_DOWN:
			down = true;
			break;
		case SDLK_LEFT:
			left = true;
			break;
		case SDLK_RIGHT:
			right = true;
			break;
		case 's':
			level.save();
			break;
		case 'l':
			level.load();
			break;
		}
	});

	window.onKeyUp([&](int keycode) {
		switch (keycode) {
		case SDLK_UP:
			up = false;
			break;
		case SDLK_DOWN:
			down = false;
			break;
		case SDLK_LEFT:
			left = false;
			break;
		case SDLK_RIGHT:
			right = false;
			break;
		}
	});

	while (!window.shouldClose()) {
		window.update();
		bitmap.clear(0xff224466);

		if (up) yOffs -= cameraMoveSpeed;
		if (down) yOffs += cameraMoveSpeed;
		if (left) xOffs -= cameraMoveSpeed;
		if (right) xOffs += cameraMoveSpeed;

		level.update();
		level.draw(bitmap, xOffs, yOffs);

		int xx = (hoveredTile.x - hoveredTile.y) * (TileSize / 2) - xOffs;
		int yy = (hoveredTile.x + hoveredTile.y) * (TileSize / 4) - yOffs;
		bitmap.blit(tiles, xx, yy, 0, TileSize, TileSize, TileSize);

		window.draw(bitmap);
	}

	return 0;
}
