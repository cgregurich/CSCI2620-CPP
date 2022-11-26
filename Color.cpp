#include "Color.h"
#include <algorithm>
#include <iostream>

Color::Color() : red(255), green(255), blue(255) {}
Color::Color(int red, int green, int blue) : red(red), green(green), blue(blue), alpha(255) {}
Color::Color(int red, int green, int blue, int alpha) : red(red), green(green), blue(blue), alpha(alpha) {}

void Color::printRGBA()
{
    // +red and so on so it prints nice
    std::cout << "Color: rgba(" << +red << ", " << +green << ", " << +blue << ", " << +alpha << ")" << std::endl;
}

// void Color::printHSV()
// {
//     std::cout << "Color: hsv(" << getHue() << ", " << getSaturation() << ", " << getValue() << ")" << std::endl;
// }

// int Color::getHue()
// // Returns between 0 and 360
// {
//     float r = red / 255.0;
//     float g = green / 255.0;
//     float b = blue / 255.0;
//     float maxRGB = std::max(std::max(r, g), b);
//     float minRGB = std::min(std::min(r, g), b);
//     float diff = maxRGB - minRGB;
//     if (maxRGB == minRGB ) return 0;

//     else if (maxRGB == r) return (int)(60 * ((g - b) / diff) + 360) % 360;
//     else if (maxRGB == g) return (int)(60 * ((b - r) / diff) + 120) % 360;
//     else if (maxRGB == b) return (int)(60 * ((r - g) / diff) + 240) % 360;
//     else return 1;

// }

// int Color::getSaturation()
// {
//     float fRed = red / 255.0;
//     float fGreen = green / 255.0;
//     float fBlue = blue / 255.0;
//     float cMax = std::max(std::max(fRed, fGreen), fBlue);
//     float cMin = std::min(std::min(fRed, fGreen), fBlue);
//     float delta = cMax - cMin;
//     float lightness = (cMax + cMin) / 2;
//     if (delta == 0) return 0;
//     return delta / (1 - std::abs(2*lightness - 1)) * 100;
// }

// int Color::getValue()
// // Returns value between 0 and 100
// {
//     float r = red / 255.0;
//     float g = green / 255.0;
//     float b = blue / 255.0;
//     float cMax = std::max(std::max(r, g), b);
//     float cMin = std::min(std::min(r, g), b);
//     return (cMax + cMin) / 2 * 100;

// }

// void Color::setRGBFromHSV(float h, float s, float v)
// {
//     std::cout << "setting: hsv(" << h << ", " << s << ", " << v << ")" << std::endl;
//     float chroma = (1 - std::abs(2*v - 1)) * s;
//     float hPrime = h / 60;
//     float x = chroma * (1 - std::abs((int)hPrime % 2 - 1));
//     float r, g, b;
//     if (hPrime <= 1)
//     {
//         r = chroma;
//         g = x;
//         b = 0;
//     }
//     else if (hPrime <= 2)
//     {
//         r = x;
//         g = chroma;
//         b = 0;
//     }
//     else if (hPrime <= 3)
//     {
//         r = 0;
//         g = chroma;
//         b = x;
//     }
//     else if (hPrime <= 4)
//     {
//         r = 0;
//         g = x;
//         b = chroma;
//     }
//     else if (hPrime <= 5)
//     {
//         r = x;
//         g = 0;
//         b = chroma;
//     }
//     else if (hPrime <= 6)
//     {
//         r = chroma;
//         g = 0;
//         b = x;
//     }
    
//     float m = v - chroma/2;
//     r += m;
//     g += m;
//     b += m;
//     this->red = (int)r * 255;
//     this->green = (int)g * 255;
//     this->blue = (int)b * 255;
// }

// void Color::setValue(int value)
// {
//     int h = getHue();
//     int s = getSaturation();
//     setRGBFromHSV(h, s, value);
// }