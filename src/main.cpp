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

	Level level(32, 32);
	Bitmap bitmap(Width, Height);
	Train car(level);

	int xOffs = 0, yOffs = 0;
	int xDrag, yDrag;
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
			car.setPosition(pos.x, pos.y);
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
		bitmap.clear(0xff224466);

		level.draw(bitmap, xOffs, yOffs);
		car.update(level);
		car.draw(bitmap, xOffs, yOffs);

		window.draw(bitmap);
	}

	return 0;
}
