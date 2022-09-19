#include <cstdio>
#include <stdint.h>
#include "Color.h"
#pragma once


enum ImageType
{
	PNG, JPG, BMP, TGA
};

struct Image 
{
	uint8_t* data = NULL;
	size_t size = 0;
	int width;
	int height;
	int channels;
	Image();
	explicit Image(const char* filename);
	Image(int width, int height, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);
	void setPixelRGB(int x, int y, int r, int g, int b);
	void setPixelColor(int x, int y, Color color);
	Color getPixelColor(int x, int y);
	ImageType getFileType(const char* filename);

	// Image Transformations
	void toGrayscale();
	void rotate180();
	void rotate90Clockwise();
	void rotate90CounterClockwise();
};