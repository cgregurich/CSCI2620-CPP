#include <cstdio>
#include <stdint.h>
#include <utility>
#include "Color.h"
#include "Point.h"
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
	void setPixelRGBA(int x, int y, int r, int g, int b, int a);
	void setPixelColor(int x, int y, Color color);
	Color getPixelColor(int x, int y);
	ImageType getFileType(const char* filename);

	// Image Transformations
	void toGrayscale();
	void applyRedFilter(int value);
	void applyGreenFilter(int value);
	void applyBlueFilter(int value);
	void rotate180();
	void rotate90Clockwise();
	void rotate90CounterClockwise();
	void translate(int deltaX, int deltaY);
	void flipHorizontally();
	void flipVertically();
	void scaleNN(float scaleFactor);
	void scaleBilinear(float scaleFactor);
	void rotateGranularUpperLeft(float degrees);
	void rotateGranularCenter(float degrees);

    void drawLine(int x1, int y1, int x2, int y2, Color color);
    void drawLine(Point a, Point b, Color color);

    void drawLineDiag(int x1, int y1, int x2, int y2, Color color);
    void drawLineDiag(Point a, Point b, Color color);

    void drawRect(int x, int y, int width, int height, Color color);
    void drawRect(Point point, int width, int height, Color color);

    void drawRectOutline(int startX, int startY, int rectWidth, int rectHeight, Color color);
    void drawRectOutline(Point point, int rectWidth, int rectHeight, Color color);

    // int calcThreshold(float thresholdRatio);
    // void threshold(float thresholdRatio);
};