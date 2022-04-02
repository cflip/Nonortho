#include "level.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

#include "train.h"
#include "util.h"

Level::Level(int width, int height, Bitmap& tileSprites)
	: m_width(width), m_height(height), m_tileSprites(tileSprites)
{
	m_tiles = new uint8_t[width * height];
	memset(m_tiles, TileGround, width * height);

	for (int i = 4; i < 14; i++)
		m_tiles[i + 6 * m_width] = TileWall;
}

uint8_t Level::get(int x, int y)
{
	if (inBounds(x, y))
		return m_tiles[x + y * m_width];
	else
		return TileGround;
}

void Level::set(int x, int y, uint8_t tile)
{
	if (inBounds(x, y))
		m_tiles[x + y * m_width] = tile;
}

void Level::update(float deltaTime)
{
	for (const auto& vehicle : m_vehicles) {
		vehicle->update(deltaTime);
	}
}

static Point2D GetSpriteForTile(uint8_t tile, int x, int y)
{
	Point2D result = { 0, 1 };
	switch (TILE_TYPE(tile)) {
	case TileGround:
		result.x = (x + y) % 2;
		result.y = 0;
		break;
	case TileWall:
		result.x = 2;
		result.y = 0;
		break;
	case TileTrack:
		result.x = TILE_DATA(tile);
		result.y = 2;
		break;
	}
	return result;
}

void Level::draw(Bitmap& bitmap, int xo, int yo)
{
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			auto tile = get(x, y);
			if (TILE_TYPE(tile) == TileWall)
				continue;

			int xx = (x - y) * (TileSize / 2) - xo;
			int yy = (x + y) * (TileSize / 4) - yo;

			Point2D textureIndex = GetSpriteForTile(tile, x, y);
			bitmap.blit(m_tileSprites, xx, yy, textureIndex.x * TileSize, textureIndex.y * TileSize, TileSize, TileSize);
		}
	}

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			auto isInTile = [x, y](const auto& vehicle) {
				return vehicle->getSpritePosition().x == x && vehicle->getSpritePosition().y == y;
			};

			auto vehiclesInTile = std::find_if(m_vehicles.begin(), m_vehicles.end(), isInTile);
			while (vehiclesInTile != m_vehicles.end()) {
				vehiclesInTile->get()->draw(bitmap, xo, yo);
				vehiclesInTile = std::find_if(++vehiclesInTile, m_vehicles.end(), isInTile);
			}

			auto tile = get(x, y);
			if (TILE_TYPE(tile) == TileWall) {
				int xx = (x - y) * (TileSize / 2) - xo;
				int yy = (x + y) * (TileSize / 4) - yo;
				Point2D textureIndex = GetSpriteForTile(tile, x, y);
				bitmap.blit(m_tileSprites, xx, yy, textureIndex.x * TileSize, textureIndex.y * TileSize, TileSize, TileSize);
			}
		}
	}
}

void Level::addVehicle(int x, int y)
{
	auto& ptr = m_vehicles.emplace_back(std::make_unique<Train>(*this));
	ptr->setPosition(x, y);
	ptr->setSpeed(0.2f);
}

void Level::toggleTile(int x, int y)
{
	static const auto updateDirection = [&](int xt, int yt) {
		if (TILE_TYPE(get(xt, yt)) == TileTrack)
			set(xt, yt, MAKE_TILE(TileTrack, ChooseDirection(*this, xt, yt)));
	};

	if (inBounds(x, y)) {
		auto tile = get(x, y);

		if (TILE_TYPE(tile) == TileTrack)
			tile = TileGround;
		else
			tile = MAKE_TILE(TileTrack, ChooseDirection(*this, x, y));

		m_tiles[x + y * m_width] = tile;

		updateDirection(x - 1, y);
		updateDirection(x + 1, y);
		updateDirection(x, y - 1);
		updateDirection(x, y + 1);
	}
}

static const char* DEFAULT_FILENAME = "level.non";
static const char* LEVEL_HEADER = "NON";
static const int LEVEL_HEADER_SIZE = 3;

void Level::save() const
{
	std::ofstream outputStream(DEFAULT_FILENAME, std::ios::out | std::ios::binary);
	if (!outputStream) {
		std::cerr << "Failed to write level to " << DEFAULT_FILENAME << '\n';
		return;
	}

	if (m_width > 255 || m_height > 255) {
		std::cerr << "Failed to save level!\n"
				  << "Level save format does not allow widths or heights greater than 255\n";
		return;
	}

	outputStream.write(LEVEL_HEADER, LEVEL_HEADER_SIZE);

	outputStream.write((char*)&m_width, 1);
	outputStream.write((char*)&m_height, 1);
	outputStream.write((char*)m_tiles, m_width * m_height);
	outputStream.close();

	std::cout << "Successfully saved level to " << DEFAULT_FILENAME << '\n';
}

void Level::load()
{
	std::ifstream inputStream(DEFAULT_FILENAME, std::ios::in | std::ios::binary);
	if (!inputStream) {
		std::cerr << "Failed to read level from " << DEFAULT_FILENAME << '\n';
		return;
	}

	char header[LEVEL_HEADER_SIZE];
	inputStream.read(header, LEVEL_HEADER_SIZE);

	if (strncmp(header, LEVEL_HEADER, LEVEL_HEADER_SIZE) != 0) {
		std::cerr << "Level loaded from " << DEFAULT_FILENAME << " does not match expected header!\n";
		return;
	}

	uint8_t width, height;
	inputStream.read((char*)&width, 1);
	inputStream.read((char*)&height, 1);

	int oldSize = m_width * m_height;
	int newSize = width * height;
	if (oldSize != newSize) {
		delete[] m_tiles;
		m_tiles = new uint8_t[newSize];
	}

	inputStream.read((char*)m_tiles, newSize);
	inputStream.close();

	m_width = width;
	m_height = height;

	printf("Successfully loaded %dx%d level from %s\n", width, height, DEFAULT_FILENAME);
}

TrackDirection ChooseDirection(Level& level, int x, int y)
{
	if (!level.inBounds(x, y)) return NorthSouth;

	bool n = TILE_TYPE(level.get(x, y - 1)) == TileTrack;
	bool e = TILE_TYPE(level.get(x + 1, y)) == TileTrack;
	bool s = TILE_TYPE(level.get(x, y + 1)) == TileTrack;
	bool w = TILE_TYPE(level.get(x - 1, y)) == TileTrack;

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
