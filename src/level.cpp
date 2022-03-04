#include "level.h"
#include "train.h"
#include "util.h"

Level::Level(int width, int height, Bitmap& tileSprites)
	: m_width(width), m_height(height), m_tileSprites(tileSprites)
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

void Level::update()
{
	for (Train* vehicle : m_vehicles) {
		if (vehicle) vehicle->update();
	}
}

void Level::draw(Bitmap& bitmap, int xo, int yo)
{
	for (int y = 0; y < 32; ++y) {
		for (int x = 0; x < 32; ++x) {
			auto tile = get(x, y);

			int xx = (x - y) * (TileSize / 2) - xo;
			int yy = (x + y) * (TileSize / 4) - yo;

			int tx = (x + y) % 2;
			int ty = 0;

			if (tile == NorthSouth) {
				tx = 0;
				ty = 2;
			} else if (tile == EastWest) {
				tx = 1;
				ty = 2;
			} else if (tile == SouthEast) {
				tx = 0;
				ty = 3;
			} else if (tile == SouthWest) {
				tx = 3;
				ty = 3;
			} else if (tile == NorthWest) {
				tx = 1;
				ty = 3;
			} else if (tile == NorthEast) {
				tx = 2;
				ty = 3;
			}

			bitmap.blit(m_tileSprites, xx, yy, tx * TileSize, ty * TileSize, TileSize, TileSize);
		}
	}

	for (Train* vehicle : m_vehicles) {
		if (vehicle) vehicle->draw(bitmap, xo, yo);
	}
}

void Level::addVehicle(Train& vehicle)
{
	m_vehicles.emplace_back(&vehicle);
}

void Level::toggleTile(int x, int y)
{
	static const auto updateDirection = [&](int xt, int yt) {
		if (get(xt, yt) > 0)
			set(xt, yt, ChooseDirection(*this, xt, yt));
	};

	if (inBounds(x, y)) {
		auto tile = get(x, y);

		if (tile > 0)
			tile = 0;
		else
			tile = ChooseDirection(*this, x, y);

		m_tiles[x + y * m_width] = tile;

		updateDirection(x - 1, y);
		updateDirection(x + 1, y);
		updateDirection(x, y - 1);
		updateDirection(x, y + 1);
	}
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