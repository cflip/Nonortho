#pragma once

#include "bitmap.h"
#include "util.h"

class Level;

class Train {
public:
	explicit Train(Level& level)
		: m_level(level) { }

	void update(float deltaTime);
	void draw(Bitmap&, int, int);

	void setPosition(int x, int y);
	Point2D getSpritePosition() const;
	void setSpeed(float speed) { m_speed = speed; };
	// Add a vehicle to the end of this train
	void addVehicle(Train*);

private:
	void findDirection();
	void findNextTile();

	Train* m_next { nullptr };
	Train* m_prev { nullptr };

	Level& m_level;

	int x { 0 }, y { 0 };
	float m_speed { 0.f };
	float m_acceleration { 0.98f };
	float m_progress { 0.f };

	Direction m_dir { North };
	int m_nextX { 0 }, m_nextY { 0 };
};
