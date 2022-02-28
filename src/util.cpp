#include "util.h"
#include <cmath>

Point2D ScreenToTile(Point2D source)
{
	Point2D result {};

	auto fx = (float)source.x;
	auto fy = (float)source.y;
	float xx = (fx / TileSize + fy / (TileSize / 2.f));
	float yy = (fy / (TileSize / 2.f) - (fx / TileSize));

	result.x = (int)floorf(xx - 1.5f); // Magic numbers
	result.y = (int)floorf(yy - 0.5f);

	return result;
}