#include "Image.h"
#include "ImageCLI.h"
#include <iostream>
#include <string>

#define HEIGHT 100
#define WIDTH 100



Image createSimpleImage(int width, int height)
{
	Image image(width, height, 3);
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			int mult = 20;
			image.setPixelRGB(x, y, mult * y, mult * y, mult * y);
		}
	}
	return image;
}


// translations
int main()
{
	Image image("simple.png");
	ImageCLI cli(image);
	cli.rotateCLI();
	cli.rgbFilterCLI();
	return 0;
}


