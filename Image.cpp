#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>



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
	read(filename);
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
	stbi_image_free(data);
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

void Image::setPixelColor(int x, int y, Color color)
{
	setPixelRGB(x, y, color.red, color.green, color.blue);
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
	memcpy(data, newImage.data, newImage.size);
}
