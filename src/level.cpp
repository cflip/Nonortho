#include "level.h"

Level::Level(int width, int height)
	: m_width(width), m_height(height)
{
	m_tiles = new uint8_t[width * height];
	memset(m_tiles, 0, width * height);
}

uint8_t Level::get(int x, int y)
{
	if (inBounds(x, y)) {
		return m_tiles[x + y * m_width];
	} else {
		return 0;
	}
}

void Level::set(int x, int y, uint8_t tile)
{
	if (inBounds(x, y))
		m_tiles[x + y * m_width] = tile;
}

RailDirection ChooseDirection(Level& level, int x, int y)
{
	if (!level.inBounds(x, y)) return NorthSouth;

	bool n = level.get(x, y - 1) > 0;
	bool e = level.get(x + 1, y) > 0;
	bool s = level.get(x, y + 1) > 0;
	bool w = level.get(x - 1, y) > 0;

	if ((n || s) && !(e || w)) {
		return NorthSouth;
	}
	if ((e || w) && !(n || s)) {
		return EastWest;
	} else if (s && e) {
		return SouthEast;
	} else if (s && w) {
		return SouthWest;
	} else if (n && w) {
		return NorthWest;
	} else if (n && e) {
		return NorthEast;
	}

	return NorthSouth;
}