#pragma once

#include "bitmap.h"
#include <cstdint>
#include <cstring>
#include <vector>

class Train;

class Level {
public:
	Level(int width, int height, Bitmap& tileSprites);
	~Level() { delete[] m_tiles; }

	uint8_t get(int x, int y);
	void set(int x, int y, uint8_t tile);
	bool inBounds(int x, int y) const { return x >= 0 && x < m_width && y >= 0 && y < m_height; }

	void update();
	void draw(Bitmap& bitmap, int xo, int yo);

	void addVehicle(Train&);
	void toggleTile(int x, int y);

private:
	Bitmap& m_tileSprites;
	int m_width, m_height;
	uint8_t* m_tiles;

	std::vector<Train*> m_vehicles;
};

enum RailDirection {
	NorthSouth = 1,
	EastWest,
	SouthEast,
	SouthWest,
	NorthWest,
	NorthEast,
};

RailDirection ChooseDirection(Level& level, int x, int y);