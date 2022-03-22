#include "bitmap.h"
#include "level.h"
#include "train.h"
#include "util.h"
#include "window.h"

int main(int argc, char** argv)
{
	constexpr int Height = 240;
	constexpr int Width = Height * 16 / 9;
	constexpr int Scale = 3;

	Window window("Nonortho", Width, Height, Scale);
	Bitmap tiles("../res/tiles.png");

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
		if (keycode == 's') {
			level.save();
		}
		if (keycode == 'l') {
			level.load();
		}
	});

	while (!window.shouldClose()) {
		window.update();
		bitmap.clear(0xff224466);

		level.update();
		level.draw(bitmap, xOffs, yOffs);

		int xx = (hoveredTile.x - hoveredTile.y) * (TileSize / 2) - xOffs;
		int yy = (hoveredTile.x + hoveredTile.y) * (TileSize / 4) - yOffs;
		bitmap.blit(tiles, xx, yy, 0, TileSize, TileSize, TileSize);

		window.draw(bitmap);
	}

	return 0;
}
