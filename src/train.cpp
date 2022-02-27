#include "train.h"

#include "bitmap.h"
#include "level.h"

void Train::update(Level& level)
{
	auto tile = level.get(x, y);
	if (tile == 0)
		return;

	if (m_progress < 1.f) {
		m_progress += m_speed;
		return;
	}

	setPosition(m_nextX, m_nextY);
}

void Train::draw(Bitmap& bitmap, int xo, int yo)
{
	float xi = ((float)x + (float)(m_nextX - x) * m_progress) * TileSize;
	float yi = ((float)y + (float)(m_nextY - y) * m_progress) * TileSize;
	int xx = (int)((xi - yi) / 2.f - (float)xo);
	int yy = (int)((xi + yi) / 4.f - (float)yo);
	bitmap.blit(m_sprite, xx, yy, 0, 0, TileSize, TileSize);
}

void Train::setPosition(int tx, int ty)
{
	x = tx;
	y = ty;

	m_progress = 0.f;
	findDirection();
	findNextTile();
}

void Train::findDirection()
{
	auto tile = m_level.get(x, y);

	if (m_dir == North) {
		if (tile == SouthEast) m_dir = East;
		if (tile == SouthWest) m_dir = West;
		if (tile == EastWest) m_dir = East;
	} else if (m_dir == East) {
		if (tile == NorthWest) m_dir = North;
		if (tile == SouthWest) m_dir = South;
		if (tile == NorthSouth) m_dir = South;
	} else if (m_dir == South) {
		if (tile == NorthEast) m_dir = East;
		if (tile == NorthWest) m_dir = West;
		if (tile == EastWest) m_dir = West;
	} else if (m_dir == West) {
		if (tile == NorthEast) m_dir = North;
		if (tile == SouthEast) m_dir = South;
		if (tile == NorthSouth) m_dir = North;
	}
}

void Train::findNextTile()
{
	if (m_dir == North) {
		m_nextX = x;
		m_nextY = y - 1;
	}
	if (m_dir == East) {
		m_nextX = x + 1;
		m_nextY = y;
	}
	if (m_dir == South) {
		m_nextX = x;
		m_nextY = y + 1;
	}
	if (m_dir == West) {
		m_nextX = x - 1;
		m_nextY = y;
	}
}