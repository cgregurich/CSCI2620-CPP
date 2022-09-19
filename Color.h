#include <stdint.h>
#include <cstdio>
#pragma once

struct Color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	Color(int red, int green, int blue);
	// void setRed(int red);
	// void setGreen(int green);
	// void setBlue(int blue);
};