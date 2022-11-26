#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <math.h>
#include <cmath>
#include <utility>

#define CHANNELS 3
#define PI 3.14159265358979323846



/*---------------------------------------------------------*/
/*--------------- CONSTRUCTORS / DESTRUCTOR ---------------*/
/*---------------------------------------------------------*/
Image::Image()
{
	std::cout << "Image()" << std::endl;
}
Image::Image(const char* filename)
{
	if (read(filename))
	{
		// printf("Read %s\n", filename);
		size = width * height * channels;
	}
	else
	{
		printf("Failed to read %s\n", filename);
	}
}

Image::Image(int width, int height, int channels) : width(width), height(height), channels(channels)
{
	size = width * height * channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.width, img.height, img.channels)
{
	memcpy(data, img.data, size);
}

Image::~Image()
{
	// stbi_image_free(data);
	delete[] data;
}



/*---------------------------------------------------------*/
/*-------------------- HELPER FUNCTIONS -------------------*/
/*---------------------------------------------------------*/

bool Image::read(const char* filename)
{
	data = stbi_load(filename, &width, &height, &channels, 0);



	return data != NULL;
}

bool Image::write(const char* filename)
{
	ImageType type = getFileType(filename);
	int success;
	switch(type)
	{
		case PNG:
			success = stbi_write_png(filename, width, height, channels, data, width*channels);
			break;
		case JPG:
			success = stbi_write_jpg(filename, width, height, channels, data, 100);
			break;
		case BMP:
			success = stbi_write_bmp(filename, width, height, channels, data);
			break;
		case TGA:
			success = stbi_write_tga(filename, width, height, channels, data);
			break;
	}
	return success != 0;
}

void Image::setPixelRGB(int x, int y, int r, int g, int b)
{
	int redIndex = y * width * channels + x * channels;
	int greenIndex = redIndex + 1;
	int blueIndex = redIndex + 2;
	data[redIndex] = r;
	data[greenIndex] = g;
	data[blueIndex] = b;
}

void Image::setPixelRGBA(int x, int y, int r, int g, int b, int a)
{
    int redIndex = y * width * channels + x * channels;
    int greenIndex = redIndex + 1;
    int blueIndex = redIndex + 2;
    int alphaIndex = redIndex + 3;
    data[redIndex] = r;
    data[greenIndex] = g;
    data[blueIndex] = b;
    data[alphaIndex] = a;
}

void Image::setPixelColor(int x, int y, Color color)
{
	setPixelRGBA(x, y, color.red, color.green, color.blue, color.alpha);
}


Color Image::getPixelColor(int x, int y)
{
	int redIndex = y * width * channels + x * channels;
	int greenIndex = redIndex + 1;
	int blueIndex = redIndex + 2;
	int r = data[redIndex];
	int g = data[greenIndex];
	int b = data[blueIndex];
	return Color(r, g, b);
}

ImageType Image::getFileType(const char* filename)
{
	const char* ext = strrchr(filename, '.');
	if (ext != nullptr)
	{
	 	if (strcmp(ext, ".png") == 0) return PNG;
	 	else if (strcmp(ext, ".jpg") == 0) return JPG;
	 	else if (strcmp(ext, ".bmp") == 0) return BMP;
	 	else if (strcmp(ext, ".tga") == 0) return TGA;
	}
	return PNG;
}

Color interpolate(Color one, Color two, float percent)
{
	float red = (1 - percent) * one.red + percent * two.red;
	float green = (1 - percent) * one.green + percent * two.green;
	float blue = (1 - percent) * one.blue + percent * two.blue;
	return Color((int)red, (int)green, (int)blue);
}

/*---------------------------------------------------------*/
/*----------------- IMAGE TRANSFORMATIONS -----------------*/
/*---------------------------------------------------------*/
void Image::toGrayscale()
{
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			Color color = getPixelColor(x, y);
			int gray = (color.red + color.green + color.blue) / 3;
			setPixelRGB(x, y, gray, gray, gray);
		}
	}
}


void Image::applyRedFilter(int value)
{
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			Color color = getPixelColor(x, y);
			setPixelRGB(x, y, std::min(color.red + value, 255), color.green, color.blue);
		}
	}
}

void Image::applyGreenFilter(int value)
{
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			Color color = getPixelColor(x, y);
			setPixelRGB(x, y, color.red, std::min(color.green + value, 255), color.blue);
		}
	}
}

void Image::applyBlueFilter(int value)
{
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			Color color = getPixelColor(x, y);
			setPixelRGB(x, y, color.red, color.green, std::min(color.blue + value, 255));
		}
	}
}

void Image::rotate180()
{
	// Treat "this" as the old image, put new transformation into newImage,
	// then copy newImage data into "this"
	Image newImage(width, height, channels);
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			Color color = getPixelColor(x, y);
			int newX = width - x - 1;
			int newY = height - y - 1;
			newImage.setPixelColor(newX, newY, color);
		}
	}
	memcpy(data, newImage.data, newImage.size);
}

void Image::rotate90Clockwise()
{
	// Treat "this" as the old image, put new transformation into newImage,
	// then copy newImage data into "this"

	// Swap height and width
	Image newImage(height, width, channels);
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			Color color = getPixelColor(x, y);
			int newX = height - y - 1;
			int newY = x;
			newImage.setPixelColor(newX, newY, color);
		}
	}
	width = newImage.width;
	height = newImage.height;
	memcpy(data, newImage.data, newImage.size);
}

void Image::rotate90CounterClockwise()
{
	// Treat "this" as the old image, put new transformation into newImage,
	// then copy newImage data into "this"

	// Swap height and width
	Image newImage(height, width, channels);
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			Color color = getPixelColor(x, y);
			int newX = y;
			int newY = width - x - 1;
			newImage.setPixelColor(newX, newY, color);
		}
	}
	width = newImage.width;
	height = newImage.height;
	memcpy(data, newImage.data, newImage.size);
}

void Image::translate(int deltaX, int deltaY)
{
	Image newImage(width, height, channels);
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			int oldY = y - deltaY;
			int oldX = x - deltaX;
			Color color(255, 0, 0);
			if (oldX >= 0 && oldX < width && oldY >= 0 && oldY < height)
			{
				color = getPixelColor(oldX, oldY);
			}
			newImage.setPixelColor(x, y, color);
		}
	}
	memcpy(data, newImage.data, newImage.size);
}

void Image::flipHorizontally()
{
	Image newImage(width, height, channels);
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			int oldX = width - x - 1;
			int oldY = y;
			Color color = getPixelColor(oldX, oldY);
			newImage.setPixelColor(x, y, color);
		}
	}
	memcpy(data, newImage.data, newImage.size);
}

void Image::flipVertically()
{
	Image newImage(width, height, channels);
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			int oldX = x;
			int oldY = height - y - 1;
			Color color = getPixelColor(oldX, oldY);
			newImage.setPixelColor(x, y, color);
		}
	}
	memcpy(data, newImage.data, newImage.size);
}

void Image::scaleNN(float scaleFactor)
{
	// Scale using nearest neighbor interpolation
	int newHeight = height * scaleFactor;
	int newWidth = width * scaleFactor;
	Image newImage(newWidth, newHeight, channels);
	for (int y=0; y<newHeight; y++)
	{
		for (int x=0; x<newWidth; x++)
		{
			int oldX = x / scaleFactor;
			int oldY = y / scaleFactor;
			Color color = getPixelColor(oldX, oldY);
			newImage.setPixelColor(x, y, color);
		}
	}
	
	delete[] data;
	data = new uint8_t[newImage.size];
	memcpy(data, newImage.data, newImage.size);
	width = newWidth;
	height = newHeight;
}

void Image::scaleBilinear(float scaleFactor)
{
	int newWidth = width * scaleFactor;
	int newHeight = height * scaleFactor;
	Image newImage(newWidth, newHeight, channels);

	for (int y=0; y<newHeight; y++)
	{
		for (int x=0; x<newWidth; x++)
		{
			/* 
			f_oldX and f_oldY are the EXACT pixels that should be pulled from
			the original image. oldX and oldY are the nearest, whole number
			pixels that actually exist. The difference between these two (i.e.
			the decimal portion of f_oldX and f_oldY) are used later to figure out
			how "close" the new image pixel is to the pixels being interpolated.
			*/
			float f_oldX = x / scaleFactor;
			float f_oldY = y / scaleFactor;
			int oldX = (int)f_oldX;
			int oldY = (int)f_oldY;


			Color color = getPixelColor(oldX, oldY);
			Color colorRight;
			Color colorDown;
			Color colorDiag;
			int nextX = oldX + 1;
			int nextY = oldY + 1;
			if (nextX < width)
			{
				colorRight = getPixelColor(nextX, oldY);
			}
			if (nextY < height)
			{
				colorDown = getPixelColor(oldX, nextY);
			}
			if (nextX < width && nextY < height)
			{
				colorDiag = getPixelColor(nextX, nextY);
			}

			float percentX = f_oldX - oldX;
			float percentY = f_oldY - oldY;

			// Interpolation of nearest pixel and its right neighbor
			Color interpolationX1 = interpolate(color, colorRight, percentX);

			// Interpolation of below pixel and its right neighbor
			Color interpolationX2 = interpolate(colorDown, colorDiag, percentX);

			// Interpolation of the above two interpolations
			Color interpolationY = interpolate(interpolationX1, interpolationX2, percentY);

			newImage.setPixelColor(x, y, interpolationY);
		}
	}
	delete[] data;
	data = new uint8_t[newImage.size];
	memcpy(data, newImage.data, newImage.size);
	width = newWidth;
	height = newHeight;
}

void Image::rotateGranularUpperLeft(float degrees)
/*
Rotate image about the upper left corner
*/
{
	Image newImage(width, height, channels);
	float radians = (float)(degrees / 360 * 2 * PI);
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			float r = sqrt(x*x + y*y);
			float theta = atan2(y, x);

			float oldAngle = theta - radians;
			float f_oldX = r * cos(oldAngle);
			float f_oldY = r * sin(oldAngle);
			int oldX = (int)f_oldX;
			int oldY = (int)f_oldY;

			Color color;
			if (oldX < 0 || oldX >= width || oldY < 0 || oldY >= height)
			{
				color = Color(255, 0, 255);
			}
			else
			{
				color = Color(getPixelColor(oldX, oldY));
				Color colorRight;
				Color colorDown;
				Color colorDiag;
				
				int nextX = oldX + 1;
				int nextY = oldY + 1;

				if (nextX < width)
				{
					colorRight = getPixelColor(nextX, oldY);
				}
				if (nextY < height)
				{
					colorDown = getPixelColor(oldX, nextY);
				}
				if (nextX < width && nextY < height)
				{
					colorDiag = getPixelColor(nextX, nextY);
				}

				float percentX = f_oldX - oldX;
				float percentY = f_oldY - oldY;

				Color interpolationX1 = interpolate(color, colorRight, percentX);
				Color interpolationX2 = interpolate(colorDown, colorDiag, percentX);
				Color interpolationY = interpolate(interpolationX1, interpolationX2, percentY);
				color = interpolationY;
			}
			newImage.setPixelColor(x, y, color);
		}
	}
	memcpy(data, newImage.data, newImage.size);
}

void Image::rotateGranularCenter(float degrees)
{
	translate(-width/2, -height/2);
	rotateGranularUpperLeft(degrees);
	translate(width/2, height/2);
}


void Image::drawLine(int x1, int y1, int x2, int y2, Color color)
{
    int lowY = std::min(y1, y2);
    int highY = std::max(y1, y2);
    int lowX = std::min(x1, x2);
    int highX = std::max(x1, x2);


    for (int y=0; y<height; y++)
    {
        for (int x=0; x<width; x++)
        {
            if (y >= lowY && y <= highY && x >= lowX && x <= highX)
            {
                setPixelColor(x, y, color);
            }
        }
    }
}

void Image::drawLine(Point a, Point b, Color color)
{
    drawLine(a.x, a.y, b.x, b.y, color);
}

int calcY(int x1, int y1, int x2, int y2, int x)
{
    int y = (y2 - y1) / (x2 - x1) * (x - x1) + y1;
    return y;
}

void Image::drawLineDiag(int x1, int y1, int x2, int y2, Color color)
{
    int lowY = std::min(y1, y2);
    int highY = std::max(y1, y2);
    int lowX = std::min(x1, x2);
    int highX = std::max(x1, x2);

    for (int x=0; x<width; x++)
    {
        int newX = x;
        int newY = calcY(x1, y1, x2, y2, newX);
        if (newY >= lowY && newY <= highY && x >= lowX && x <= highX)
        {
            setPixelColor(newX, newY, color);
        }
    }
}

void Image::drawLineDiag(Point a, Point b, Color color)
{
    drawLineDiag(a.x, a.y, b.x, b.y, color);
}

void Image::drawRect(int startX, int startY, int rectWidth, int rectHeight, Color color)
{
    for (int y=startY; y<startY + rectHeight; y++)
    {
        for (int x=startX; x<startX + rectWidth; x++)
        {
            setPixelColor(x, y, color);
        }
    }
}

void Image::drawRect(Point point, int rectWidth, int rectHeight, Color color)
{
    for (int y=point.y; y<point.y + rectHeight; y++)
    {
        for (int x=point.x; x<point.x + rectWidth; x++)
        {
            setPixelColor(x, y, color);
        }
    }
}

void Image::drawRectOutline(int startX, int startY, int rectWidth, int rectHeight, Color color)
{
    int upperLeftX = startX;
    int upperLeftY = startY;
    int upperRightX = startX + rectWidth;
    int upperRightY = startY;
    int lowerLeftX = startX;
    int lowerLeftY = startY + rectHeight;
    int lowerRightX = startX + rectWidth;
    int lowerRightY = startY + rectHeight;

    drawLine(upperLeftX, upperLeftY, upperRightX, upperRightY, color);
    drawLine(upperLeftX, upperLeftY, lowerLeftX, lowerLeftY, color);
    drawLine(upperRightX, upperRightY, lowerRightX, lowerRightY, color);
    drawLine(lowerLeftX, lowerLeftY, lowerRightX, lowerRightY, color);
}

void Image::drawRectOutline(Point point, int rectWidth, int rectHeight, Color color)
{
    drawRectOutline(point.x, point.y, rectWidth, rectHeight, color);
}

// int Image::calcThreshold(float thresholdRatio)
// {

//     int min = 256;
//     int max = -1;
//     for (int y=0; y<height; y++)
//     {
//         for (int x=0; x<width; x++)
//         {
//             int value = getPixelColor(x, y).getValue();
//             min = std::min(min, value);
//             max = std::max(max, value);
//         }
//     }
//     return (min + max) * thresholdRatio;
// }

// void Image::threshold(float thresholdRatio)
// /* thresholdRatio supposed to be between 0.0 and 1.0
//  0.5 uses the average of the min and max value
//  < 0.5 = lighter
//  > 0.5 = darker
// */
// {
//     int threshold = calcThreshold(thresholdRatio);
//     Color white(255, 255, 255);
//     Color black(0, 0, 0);
//     for (int y=0; y<height; y++)
//     {
//         for (int x=0; x<width; x++)
//         {
//             Color c = getPixelColor(x, y);
//             if (c.getValue() >= threshold) setPixelColor(x, y, white);
//             else setPixelColor(x, y, black);
//         }
//     }
// }