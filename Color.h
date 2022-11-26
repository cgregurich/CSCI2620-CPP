#include <stdint.h>
#include <cstdio>
#pragma once

struct Color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
    uint8_t alpha;
	
	Color();
	Color(int red, int green, int blue);
    Color(int red, int green, int blue, int alpha);

    // int getValue();
    // int getSaturation();
    // int getHue();

    void printRGBA();
    void printHSV();

    void setValue(int value);
    private:
        void setRGBFromHSV(float h, float s, float v);
};