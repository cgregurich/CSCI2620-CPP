#include "ImageCLI.h"
#include "Image.h"
#include <iostream>
#include <string>

/*
----------------------------------------------
------------- Constructors ---------------------
------------------------------------------------
*/

ImageCLI::ImageCLI(Image image) : image(image)
{
	image.write("done.png");
}
ImageCLI::ImageCLI(const char* filename) : image(filename)
{
	image.write("done.png");
}



/*
----------------------------------------------
------------- CLI Programs--------------------
----------------------------------------------
*/
void ImageCLI::rotateCLI()
{
	while (true)
	{
		std::cout << "Enter command: ";
		std::string cmd;
		std::cin >> cmd;
		if (cmd == "180")
		{
			image.rotate180();
		}
		else if (cmd == "-90")
		{
			image.rotate90CounterClockwise();
		}
		else if (cmd == "90")
		{
			image.rotate90Clockwise();
		}
		else if (cmd == "q")
		{
			return;
		}
		else {
			std::cout << "command not recognized" << std::endl;
		}
		image.write("done.png");
	}
}

void ImageCLI::rgbFilterCLI()
{
	while (true)
	{

		std::cout << "Enter r, g, or b: ";
		char cmd;
		std::cin >> cmd;
		if (cmd == 'q') return;

		int filterValue;
		std::cout << "Enter filter value: ";
		std::string input;
		std::cin >> input;
		filterValue = std::stoi(input);

		if (cmd == 'r') applyRedFilter(filterValue);
		else if (cmd == 'g') applyGreenFilter(filterValue);
		else if (cmd == 'b') applyBlueFilter(filterValue);
		else if (cmd == 'q') return;

		image.write("done.png");
	}
}


/*
----------------------------------------------
------------- Helper Functions ---------------
----------------------------------------------
*/
int clamp(int num, int max)
{
	if (num > max) return max;
	return num;
}

void ImageCLI::applyRedFilter(int value)
{
	for (int y=0; y<image.height; y++)
	{
		for (int x=0; x<image.width; x++)
		{
			Color color = image.getPixelColor(x, y);
			image.setPixelRGB(x, y, clamp(color.red + value, 255), color.green, color.blue);
		}
	}
}

void ImageCLI::applyGreenFilter(int value)
{
	for (int y=0; y<image.height; y++)
	{
		for (int x=0; x<image.width; x++)
		{
			Color color = image.getPixelColor(x, y);
			image.setPixelRGB(x, y, color.red, clamp(color.green + value, 255), color.blue);
		}
	}
}

void ImageCLI::applyBlueFilter(int value)
{
	for (int y=0; y<image.height; y++)
	{
		for (int x=0; x<image.width; x++)
		{
			Color color = image.getPixelColor(x, y);
			image.setPixelRGB(x, y, color.red, color.green, clamp(color.blue + value, 255));
		}
	}
}