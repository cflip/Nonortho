#include "train.h"

#include "level.h"
#include "bitmap.h"

void TrainCar::update(Level& level)
{
	auto tile = level.get(x, y);
	if (tile == 0)
		return;

	if (m_progress < 1.f) {
		m_progress += m_speed;
		return;
	}

	m_progress = 0.f;

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

    nextTile();
}

void TrainCar::draw(Bitmap& bitmap, int xo, int yo)
{
	float xi = ((float)x + (float)(m_nextX - x) * m_progress) * 24;
	float yi = ((float)y + (float)(m_nextY - y) * m_progress) * 24;
	int xx = (int)((xi - yi) / 2.f - (float)xo);
	int yy = (int)((xi + yi) / 4.f - (float)yo);
	bitmap.blit(m_sprite, xx, yy, 0, 0, 24, 24);
}

void TrainCar::setPosition(int tx, int ty)
{
    x = tx;
    y = ty;
    m_progress = 0.f;
    nextTile();
}

void TrainCar::nextTile()
{
    if (m_dir == North) { m_nextX = x; m_nextY = y - 1; }
    if (m_dir == East) { m_nextX = x + 1; m_nextY = y; }
    if (m_dir == South) { m_nextX = x; m_nextY = y + 1; }
    if (m_dir == West) { m_nextX = x - 1; m_nextY = y; }
}