#include "Image.h"
#include "ImageCLI.h"
#include <iostream>
#include <chrono>

#include <string>
#include <math.h>
#include <vector>
#include "Kernel.h"


int interpolate(int n1, int n2)
{
    return (n1 + n2) / 2;
}

Color combineColors(Color c1, Color c2)
{
    int newRed = interpolate(c1.red, c2.red);
    int newGreen = interpolate(c1.green, c2.green);
    int newBlue = interpolate(c1.blue, c2.blue);
    return Color(newRed, newGreen, newBlue);
}
    /*
    Todo/doing:
    - combining colors; need to figure out how alpha plays into it, and add alpha to my color class (default value of 1?)? channels?
    - putting images next to each other (fun math with scaling and shit)
    - layering
    - mosaics!
    - might need a cropping function? Or just make sure to find images with the same aspect ratios
    https://stackoverflow.com/questions/726549/algorithm-for-additive-color-mixing-for-rgb-values
    */

Image makeTestImage(Color c)
{
    int w = 100;
    int h = 100;
    Image image(w, h, 4);
    for (int y=0; y<h; y++)
    {
        for (int x=0; x<w; x++)
        {
            image.setPixelColor(x, y, c);
        }
    }
    return image;

}
Color blendColors(Color fg, Color bg)
{
    /* 
    My Color class stores all four channels in range of 0-255, but 
    the formula I use for blending colors uses the range of 0.0-1.0,
    so convert all the channels for both colors to 0.0-1.0.
    */
    Color result(0, 0, 0, 0);
    float fgRed = fg.red / 255.0;
    float fgGreen = fg.green / 255.0;
    float fgBlue = fg.blue / 255.0;
    float fgAlpha = fg.alpha / 255.0;

    float bgRed = bg.red / 255.0;
    float bgGreen = bg.green / 255.0;
    float bgBlue = bg.blue / 255.0;
    float bgAlpha = bg.alpha / 255.0;

    float resRed = 0;
    float resGreen = 0;
    float resBlue = 0;
    float resAlpha = 0;

    resAlpha = 1 - (1 - fgAlpha) * (1 - bgAlpha);
    if (resAlpha < 0.000001) return result; // TODO: what should be returned?
    resRed = fgRed * fgAlpha / resAlpha + bgRed * bgAlpha * (1 - fgAlpha) / resAlpha;
    resGreen = fgGreen * fgAlpha / resAlpha + bgGreen * bgAlpha * (1 - fgAlpha) / resAlpha;
    resBlue = fgBlue * fgAlpha / resAlpha + bgBlue * bgAlpha * (1 - fgAlpha) / resAlpha;
    result.red = resRed * 255;
    result.green = resGreen * 255;
    result.blue = resBlue * 255;
    result.alpha = resAlpha * 255;
    return result;
}

void paintThreeSwatches(Color a, Color b, Color c)
{
    int swatchWidth = 100;
    int swatchHeight = 100;
    int swatches = 3;

    Image image(swatchWidth*swatches, swatchHeight, 4);

    for (int swatch=0; swatch<swatches; swatch++)
    {
        for (int y=0; y<swatchHeight; y++)
        {
            for (int x=swatch*swatchWidth; x<(swatch*swatchWidth) + swatchWidth; x++)
            {
                if (swatch == 0) image.setPixelColor(x, y, a);
                else if (swatch == 1) image.setPixelColor(x, y, b);
                else if (swatch == 2) image.setPixelColor(x, y, c);
            }
        }
    }

    image.write("images/blended.png");
}

Image cropImage(Image image, int xDiff, int yDiff)
{
    Image cropped(image.width - 2 * xDiff, image.height - 2 * yDiff, 4);
    for (int y=0; y<cropped.height; y++)
    {
        for (int x=0; x<cropped.width; x++)
        {
            Color c = image.getPixelColor(x+xDiff, y+yDiff);
            cropped.setPixelColor(x, y, c);
        }
    }
    return cropped;
}

Image scaleImageToPixels(Image image, int pixels)
{
    float scaleFactor = 1;
    if (image.width <= image.height)
    {
        // Need to scale based on width
        scaleFactor = (float)pixels / image.width;
    }
    else
    {
        // Need to scale based on height
        scaleFactor = (float)pixels / image.height;
    }
    image.scaleBilinear(scaleFactor);
    return image;
}

Image makeTile(Image image, int tileSize)
{
    Image scaled = scaleImageToPixels(image, tileSize);
    int xDiff = ((float)scaled.width - (float)tileSize) / 2;
    int yDiff = ((float)scaled.height - (float)tileSize) / 2;
    Image cropped = cropImage(scaled, xDiff, yDiff);
    return cropped;
}



Image makeMosaic(std::vector<Image> images)
{
    /*
    Collage consists of images. So a collage has a grid, which each "square" in 
    the grid, aka each tile, is an image which itself is a grid.
    Collage consists of tiles, tiles consist of pixels.
    */
    Image mainImage("images/eiffel.png");

    // # of tiles that make up the collage
    int TILES_HORIZONTAL = mainImage.height; 
    int TILES_VERTICAL = mainImage.width; 


    // Assuming images are all exact same dimensions
    // Total pixels in the collage
    int pixelsHorizontal = images[0].width * TILES_HORIZONTAL;
    int pixelsVertical = images[0].height * TILES_VERTICAL;
    int channels = images[0].channels;


    int tilePixelsWidth = images[0].width;
    int tilePixelsHeight = images[0].height;


    Image mosaic(pixelsHorizontal, pixelsVertical, channels);

    // tileY and tileX is the coordinates of the tiles in the mosaic
    for (int tileY=0; tileY<TILES_VERTICAL; tileY++)
    {
        for (int tileX=0; tileX<TILES_HORIZONTAL; tileX++)
        {
            int imageIndex = tileY * TILES_HORIZONTAL + tileX;
            if (imageIndex >= images.size())
            {
                return mosaic;
            }
            Image image = images[imageIndex];
            Color filter = mainImage.getPixelColor(tileX, tileY);
            filter.alpha = 127;
            
            // tilePixelY and tilePixelX are the pixels relative to each tile
            // each tile will have pixels at the same coordinates
            // i.e. even if it's the tile on the far right of the mosaic, the
            // first pixel in the tile will still be at (0, 0)
            for (int tilePixelY=0; tilePixelY<tilePixelsHeight; tilePixelY++)
            {
                for (int tilePixelX=0; tilePixelX<tilePixelsWidth; tilePixelX++)
                {
                    // x and y are the pixel coordinates in the overall mosaic
                    // i.e. not relative to a tile, but to the mosaic image itself.
                    int x = tileX * tilePixelsWidth + tilePixelX;
                    int y = tileY * tilePixelsHeight + tilePixelY;
                    
                    // Color color = image.getPixelColor(tilePixelX, tilePixelY);
                    Color c = image.getPixelColor(tilePixelX, tilePixelY);
                    Color blended = blendColors(filter, c);

                    mosaic.setPixelColor(x, y, blended);
                }
            }
        }
    }
    return mosaic;
}


Image threeChannelsToFourChannels(Image image)
{
    Image newImage(image.width, image.height, 4);
    for (int y=0; y<newImage.height; y++)
    {
        for (int x=0; x<newImage.width; x++)
        {
            newImage.setPixelColor(x, y, image.getPixelColor(x, y));
        }
    }
    return newImage;

}

std::vector<Image> makeTiles(int count)
{
    std::vector<Image> images;
    while (count--)
    {
        images.push_back(Image("images/eiffel.png"));
    }
    return images;
}


int main()
{

    // threeChannelsToFourChannels(Image("images/eiffel.jpg")).write("images/eiffel.png");

    // int dimensions = 100;
    // std::vector<Image> images = makeTiles(dimensions * dimensions);
    // Image mosaic = makeMosaic(images);
    // std::cout << "done" << std::endl;
    // mosaic.write("images/collage.png");
    Image image("images/collage.png");
    Kernel kernel;
    kernel.ridge(image).write("images/kernel.png");


	return 0;
}