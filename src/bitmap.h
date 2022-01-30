#pragma once

#include <cstdint>

class Bitmap {
public:
	Bitmap(int width, int height)
		: width(width), height(height)
	{
		data = new uint32_t[width * height];
	}

	Bitmap(const char* image);

	~Bitmap() { delete[] data; }

	void clear(uint32_t colour);
	void blit(Bitmap const& other, int x, int y, int xc, int yc, int w, int h);

	uint32_t* data;
	int width, height;
};