#include "Image.h"
#include <vector>

struct Kernel
{
    Image applyKernel(Image image, std::vector<std::vector<int>> kernel, float divideBy);
    Image vertical(Image image);
    Image horizontal(Image image);
    Image sharpen(Image image);
    Image meanBlur(Image image);
    Image ridge(Image image);
    Image sobel(Image image);


};