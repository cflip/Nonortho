#pragma once

#include <cmath>

struct Point2D {
	int x, y;
};

enum Direction {
	North,
	East,
	South,
	West
};

constexpr int TileSize = 24;

/*
 * Converts screen coordinates to a tile on the map
 */
Point2D ScreenToTile(Point2D);