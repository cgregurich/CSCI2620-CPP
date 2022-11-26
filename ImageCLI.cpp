#include "ImageCLI.h"
#include "Image.h"
#include <iostream>
#include <string>
#include <algorithm>


std::string getUserInput(std::string);
int getUserInt(std::string prompt);
/*
----------------------------------------------
------------- Constructors ---------------------
------------------------------------------------
*/

ImageCLI::ImageCLI(Image image, std::string donePath) : image(image), donePath(donePath)
{
	image.write(donePath.c_str());
}
ImageCLI::ImageCLI(std::string filename, std::string donePath) : image(filename.c_str()), donePath(donePath)
{
	image.write(donePath.c_str());
}


void ImageCLI::save()
{
	image.write(donePath.c_str());
}


/*
----------------------------------------------
------------- CLI Programs--------------------
----------------------------------------------
*/
void ImageCLI::rotate()
{
	while (true)
	{

		std::string cmd = getUserInput("ROTATE: Enter rotation amount: ");
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
		save();
	}
}

void ImageCLI::rgbFilter()
{
	while (true)
	{
		std::string cmd = getUserInput("RGB: Enter r, g, or b: ");
		if (cmd == "q") return;

		std::string valueStr = getUserInput("Enter filter value: ");
		if (valueStr == "q") return;
		int value = std::stoi(valueStr);

		if (cmd == "r") image.applyRedFilter(value);
		else if (cmd == "g") image.applyGreenFilter(value);
		else if (cmd == "b") image.applyBlueFilter(value);

		save();
	}
}

void ImageCLI::translate()
{
	while (true)
	{
		int deltaX = std::stoi(getUserInput("TRANSLATE: Enter x value: "));
		int deltaY = std::stoi(getUserInput("TRANSLATE: Enter y value: "));

		image.translate(deltaX, deltaY);
		
		save();
	}
}

void ImageCLI::flip()
{
	while(true)
	{
		std::string cmd = getUserInput("FLIP: Enter h or v: ");
		if (cmd == "q") return;
		else if (cmd == "h") image.flipHorizontally();
		else if (cmd == "v") image.flipVertically();
		save();
	}
}

void ImageCLI::scale()
{
	while (true)
	{
		std::string cmd = getUserInput("SCALE: Enter scale factor: ");
		if (cmd == "q") return;
		float scaleFactor = std::stof(cmd);
		image.scaleNN(scaleFactor);
		save();

	}
}

void ImageCLI::drawLine()
{
    while (true)
    {
        int x1 = getUserInt("LINE: Enter x1: ");
        int y1 = getUserInt("LINE: Enter y1: ");
        int x2 = getUserInt("LINE: Enter x2: ");
        int y2 = getUserInt("LINE: Enter y2: ");
        image.drawLine(x1, y1, x2, y2, Color(0, 255, 0));
        save();
    }
}

void ImageCLI::drawRectOutline()
{
    while (true)
    {
        int x = getUserInt("RECT OUTLINE: Enter x: ");
        int y = getUserInt("RECT OUTLINE: Enter y: ");
        int rectWidth = getUserInt("RECT OUTLINE: Enter width: ");
        int rectHeight = getUserInt("RECT OUTLINE: Enter height: ");
        image.drawRectOutline(x, y, rectWidth, rectHeight, Color(255, 0, 0));
        save();
    }
}

void ImageCLI::drawRect()
{
    while (true)
    {
        int x = getUserInt("RECT: Enter x: ");
        int y = getUserInt("RECT: Enter y: ");
        int rectWidth = getUserInt("RECT: Enter width: ");
        int rectHeight = getUserInt("RECT: Enter height: ");
        image.drawRect(x, y, rectWidth, rectHeight, Color(255, 0, 0));
        save();
        break;
    }
}


/*
----------------------------------------------
------------- Helper Functions ---------------
----------------------------------------------
*/
std::string getUserInput(std::string prompt)
{
	std::string input;
	std::cout << prompt;
	std::cin >> input;
	return input;
}

int getUserInt(std::string prompt)
{
    std::string input;
    std::cout << prompt;
    std::cin >> input;
    int n = std::stoi(input);
    return n;
}
