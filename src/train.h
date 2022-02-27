#pragma once

#include "bitmap.h"
#include "util.h"

class Level;

class Train {
public:
	explicit Train(Level& level)
		: m_sprite("../res/car.png"), m_level(level) { }

	void update(Level&);
	void draw(Bitmap&, int, int);

	void setPosition(int x, int y);

private:
	void findDirection();
	void findNextTile();

	Bitmap m_sprite;
	Level& m_level;

	int x { 0 }, y { 0 };
	float m_speed { 0.05f };
	float m_progress { 0.f };

	Direction m_dir { North };
	int m_nextX { 0 }, m_nextY { 0 };
};
