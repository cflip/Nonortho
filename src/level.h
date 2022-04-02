#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "bitmap.h"

#define TILE_TYPE(x) ((x)&0xf)
#define TILE_DATA(x) (x >> 4 & 0xf)
#define MAKE_TILE(t, d) ((((d)&0xf) << 4) + ((t)&0xf))

class Train;

enum TileType : uint8_t {
	TileGround,
	TileWall,
	TileTrack
};

enum TrackDirection : uint8_t {
	NorthSouth,
	EastWest,
	SouthEast,
	SouthWest,
	NorthWest,
	NorthEast,
};

class Level {
public:
	Level(int width, int height, Bitmap& tileSprites);
	~Level() { delete[] m_tiles; }

	uint8_t get(int x, int y);
	void set(int x, int y, uint8_t tile);
	bool inBounds(int x, int y) const { return x >= 0 && x < m_width && y >= 0 && y < m_height; }

	void update(float deltaTime);
	void draw(Bitmap& bitmap, int xo, int yo);

	void addVehicle(int x, int y);
	void toggleTile(int x, int y);

	void save() const;
	void load();

private:
	Bitmap& m_tileSprites;
	int m_width, m_height;
	uint8_t* m_tiles;

	std::vector<std::unique_ptr<Train>> m_vehicles;
};

TrackDirection ChooseDirection(Level& level, int x, int y);
