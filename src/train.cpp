#include "train.h"

#include "level.h"
#include "bitmap.h"

void TrainCar::update(Level& level)
{
	auto tile = level.get(x, y);
	if (tile == 0)
		return;

	if (m_progress < 12) {
		m_progress++;
		return;
	}
	else {
		m_progress = 0;
	}

	auto n = level.get(x, y - 1);
	auto e = level.get(x + 1, y);
	auto s = level.get(x, y + 1);
	auto w = level.get(x - 1, y);

	if (m_dir == North) {
		if (n == SouthEast) m_dir = East;
		if (n == SouthWest) m_dir = West;
		y--;
	}
	else if (m_dir == East) {
		if (e == NorthWest) m_dir = North;
		if (e == SouthWest) m_dir = South;
		x++;
	}
	else if (m_dir == South) {
		if (s == NorthEast) m_dir = East;
		if (s == NorthWest) m_dir = West;
		y++;
	}
	else if (m_dir == West) {
		if (w == NorthEast) m_dir = North;
		if (w == SouthEast) m_dir = South;
		x--;
	}
}

void TrainCar::draw(Bitmap& bitmap, int xo, int yo)
{
	int xx = (x - y) * (24 / 2) - xo;
	int yy = (x + y) * (24 / 4) - yo;
	bitmap.blit(m_sprite, xx, yy, 0, 0, 24, 24);
}