#include "Kernel.h"

Image Kernel::applyKernel(Image image, std::vector<std::vector<int>> kernel, float divideBy)
{

    Image newImage(image.width, image.height, image.channels);
    for (int y=0; y<image.height; y++)
    {
        for (int x=0; x<image.width; x++)
        {
            Color color = image.getPixelColor(x, y);

            int newRed = 0;
            int newGreen = 0;
            int newBlue = 0;

            for (int r=0; r<3; r++)
            {
                for (int c=0; c<3; c++)
                {
                    int rowDiff = r - 1;
                    int colDiff = c - 1;
                    int adjacentX = x + colDiff;
                    int adjacentY = y + rowDiff;

                    Color adjacentColor;
                    if (adjacentY >= 0 && adjacentY < image.height && adjacentX >= 0 && adjacentX < image.width)
                    {
                        adjacentColor = image.getPixelColor(adjacentX, adjacentY);

                    }
                    newRed += kernel[r][c] * adjacentColor.red;
                    newGreen += kernel[r][c] * adjacentColor.green;
                    newBlue += kernel[r][c] * adjacentColor.blue;

                }
            }

            newRed /= divideBy;
            newGreen /= divideBy;
            newBlue /= divideBy;

            newRed = std::max(newRed, 0);
            newGreen = std::max(newGreen, 0);
            newBlue = std::max(newBlue, 0);

            newRed = std::min(newRed, 255);
            newGreen = std::min(newGreen, 255);
            newBlue = std::min(newBlue, 255);
            Color newColor(newRed, newGreen, newBlue);
            newImage.setPixelColor(x, y, newColor);

        }
    }
    return newImage;
}

Image Kernel::vertical(Image image)
{
    std::vector<std::vector<int>> kernel {
        {1, 0, -1},
        {1, 0, -1},
        {1, 0, -1},
    };
    return applyKernel(image, kernel, 1);
}

Image Kernel::horizontal(Image image)
{
    std::vector<std::vector<int>> kernel {
        {1, 1, 1},
        {0, 0, 0},
        {-1, -1, -1},
    };
    return applyKernel(image, kernel, 1);
}

Image Kernel::sharpen(Image image)
{
    std::vector<std::vector<int>> kernel {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0},
    };
    return applyKernel(image, kernel, 1);
}

Image Kernel::meanBlur(Image image)
{
    std::vector<std::vector<int>> kernel {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    return applyKernel(image, kernel, 9);
}

Image Kernel::ridge(Image image)
{
    std::vector<std::vector<int>> kernel {
        {-1, -1, -1},
        {-1, 8, -1},
        {-1, -1, -1}
    };
    return applyKernel(image, kernel, 1);
}

Image Kernel::sobel(Image image)
{
    std::vector<std::vector<int>> gx {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1},
    };

    std::vector<std::vector<int>> gy {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1},
    };

    Image i = applyKernel(image, gx, 1);
    return applyKernel(i, gy, 1);
}
