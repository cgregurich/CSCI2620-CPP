#include "Image.h"
#pragma once

struct ImageCLI
{
	Image image;
	ImageCLI(const char* filename);
	ImageCLI(Image sourceImage);

	void rotateCLI();
	void rgbFilterCLI();

	void applyRedFilter(int value);
	void applyGreenFilter(int value);
	void applyBlueFilter(int value);
};