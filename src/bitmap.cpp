#include "bitmap.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Bitmap::Bitmap(const char* image)
{
	int width, height, bitDepth;

	uint8_t* buffer = stbi_load(image, &width, &height, &bitDepth, STBI_rgb_alpha);
	if (!buffer) {
		return;
	}

	this->width = width;
	this->height = height;
	data = new uint32_t[width * height];

	for (int i = 0; i < width * height; i++) {
		uint8_t r = buffer[i * 4 + 0];
		uint8_t g = buffer[i * 4 + 1];
		uint8_t b = buffer[i * 4 + 2];
		uint8_t a = buffer[i * 4 + 3];

		data[i] = (a << 24) | (r << 16) | (g << 8) | b;
	}

	stbi_image_free(buffer);
}

void Bitmap::clear(uint32_t colour)
{
	memset(data, colour, width * height * 4);
}

void Bitmap::blit(Bitmap const& other, int xo, int yo, int xc, int yc, int w, int h)
{
	constexpr int MaskColour = 0xffff00ff;

	for (int y = 0; y < h; ++y) {
		int yp = y + yo;
		if (yp < 0 || yp >= height) continue;

		for (int x = 0; x < w; ++x) {
			int xp = x + xo;
			if (xp < 0 || xp >= width) continue;

			int src = other.data[(x + xc) + (y + yc) * other.width];
			if (src != MaskColour)
				data[xp + yp * width] = src;
		}
	}
}
