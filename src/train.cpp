#include "train.h"

#include <cmath>

#include "bitmap.h"
#include "level.h"

static const Bitmap SPRITES("res/car.png");

void Train::update()
{
	if (m_next) {
		m_speed = m_next->m_speed;
		m_progress = m_next->m_progress;
	} else {
		auto tile = m_level.get(x, y);
		if (TILE_TYPE(tile) != TileTrack)
			return;
	}

	m_speed *= m_acceleration;
	if (m_progress < 1.f) {
		m_progress += m_speed;
		return;
	}

	if (m_next) {
		x = m_nextX;
		y = m_nextY;
		m_nextX = m_next->x;
		m_nextY = m_next->y;
		m_progress = 0.f;
		findDirection();
	}

	setPosition(m_nextX, m_nextY);
}

void Train::draw(Bitmap& bitmap, int xo, int yo)
{
	float xi = ((float)x + (float)(m_nextX - x) * m_progress) * TileSize;
	float yi = ((float)y + (float)(m_nextY - y) * m_progress) * TileSize;
	int xx = (int)((xi - yi) / 2.f - (float)xo);
	int yy = (int)((xi + yi) / 4.f - (float)yo);

	int tx = 0;
	if (m_dir == East || m_dir == West) tx = 24;

	bitmap.blit(SPRITES, xx, yy, tx, 0, TileSize, TileSize);
}

void Train::setPosition(int tx, int ty)
{
	x = tx;
	y = ty;
	m_progress = 0.f;

	findDirection();
	findNextTile();
}

Point2D Train::getSpritePosition() const
{
	int xi = ceil((float)x + (float)(m_nextX - x) * m_progress);
	int yi = ceil((float)y + (float)(m_nextY - y) * m_progress);
	return { xi, yi };
}

void Train::addVehicle(Train* newTrain)
{
	if (!m_prev) {
		m_prev = newTrain;
		newTrain->m_next = this;
	} else {
		m_prev->addVehicle(newTrain);
	}
}

void Train::findDirection()
{
	auto dir = TILE_DATA(m_level.get(x, y));

	if (m_dir == North) {
		if (dir == SouthEast) m_dir = East;
		if (dir == SouthWest) m_dir = West;
		if (dir == EastWest) m_dir = East;
	} else if (m_dir == East) {
		if (dir == NorthWest) m_dir = North;
		if (dir == SouthWest) m_dir = South;
		if (dir == NorthSouth) m_dir = South;
	} else if (m_dir == South) {
		if (dir == NorthEast) m_dir = East;
		if (dir == NorthWest) m_dir = West;
		if (dir == EastWest) m_dir = West;
	} else if (m_dir == West) {
		if (dir == NorthEast) m_dir = North;
		if (dir == SouthEast) m_dir = South;
		if (dir == NorthSouth) m_dir = North;
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
