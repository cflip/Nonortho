#pragma once

#include "bitmap.h"

enum CarDirection {
	North,
	East,
	South,
	West
};

class Level;

class Train {
public:
	Train()
		: m_sprite("../res/car.png") {}

	void update(Level&);
	void draw(Bitmap&, int, int);

    void setPosition(int x, int y);
private:
    void nextTile();

	Bitmap m_sprite;

    int x{0}, y{0};
    float m_speed{0.05f};
	float m_progress{ 0.f };

	CarDirection m_dir{ North };
    int m_nextX{0}, m_nextY{0};
};
