#include "Image.h"
#include <string>
#pragma once

struct ImageCLI
{
	Image image;
	std::string donePath; // Path of image to write changes to
	ImageCLI(std::string filename, std::string donePath);
	ImageCLI(Image sourceImage, std::string donePath);

	void save();

	void rotate();
	void rgbFilter();
	void translate();
	void flip();
	void scale();

    void drawLine();
    void drawRectOutline();
    void drawRect();

};