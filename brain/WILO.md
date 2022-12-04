filter isn't great because it bases each application of a filter off the image as is, not the image in the state that it was before any filter was applied. Not sure it really matters tho.
Still could add the thing where you pass the done filename into the ImageCLI

implemented translation CLI. look at scaling. also refactor CLI so you can have more of a menu. maybe use commands to do things?

working on scaling. moved the scaling logic to the image class. the problem with this is when trying to essentially replace "this" with the newImage. If the image was scaled up, then newImage is larger than "this", so memcpy doesn't work. If the image was scaled down, then it works, but there's still old data in "this", which of course is a larger size than the new image. hmmm.

10/3
trying to draw diagonal line

10/8:
implemented draw line diag (not sure how it works with imperfect diagonals), draw rect, draw rect outline, and made a point class and extra methods for these draw methods that use points instead of x y coords.
need to write cli for these, and then implement histogram.
also weirdness: when drawing lines on, say, archery.png, I can SEE the lines drawn in the output image. How is this happening? I'm making all the transformations THEN saving the image. Also weird how this isn't visible with drawRect. Just drawRectOutline

11/14:
- make kernel cli
- mosaic
- how to put multiple smaller images in a new image

LOGS
10/24:
worked on getting HSV to RGB to work so I can implement kernel convolutions. Fixed hue (the gfg article explained it poorly and only by looking at the code did I see the mistake they made in the explanation).
However, I realized that the saturation and value functions aren't working properly. Might have to do with using 1-100 but it really shouldn't. it's the same as a percentage, after all

11/6:
realized that the kernels isn't supposed to be applied to the "value" (i.e. hsv) which is good, because I was having issues with HSV vs HSL (didn't know they were different). So I modified my applyKernel function to interact with the color channels instead of the HSV value. This made an immediately noticeable difference in how my filtered images were coming out. Spent some time just messing around with different images and seeing how the effects turn out. Also split the kernel application process into their own functions (sharpen, horizontal, vertical, etc.) I'd like to look into the ridge detection one because just using the matrix given by wikipedia doesn't seem to give the same effect, so there must be more to it. Also my sharpen doesn't seem to be working properly. It has an effect, but I wouldn't say it makes the image look "sharper"

11/7: saw one of those pics created with a bunch of tiny pictures as the "pixels". want to look into that. Also dithering. also tested sharpen on monkey pic, it looks like it works.



week of 11/13
11/14: moved kernel code into its own kernel class.

thanksgiving break
spent time working on the basics of making a collage, ignoring the mosaic (i.e. color filter/layering) aspect.
Just wanted to be able to take a collection of images, and lay them in a grid inside another image.
This had some challenges. First I need to make sure that all the images are the same size. So I wrote functions to scale images to a given pixel width/height, and another one to crop the images to a certain width/height.
For the most part, it works. There is some weirdness that happens sometimes when I try to scale to a certain pixel size, including rounding issues where I'll try to scale/crop an image to be, say, 400 pixels, and it will end up 401 or 399 pixels. I'm also not totally sure what the relationship between scaling and cropping is. Which one should be applied first? How much do I scale vs crop to get the desired image dimensions without cutting off the main content of the image? I pivoted away from these problems for a bit and started working on a function that creates a collage out of uniform, square images so I don't need to worry about the "off by one pixel" issue. This took a long time to do because of one interesting issue, and one not so interesting issue. The first one is that I was treating a 3-channel image as a 4-channel image without realizing it. The image I was writing to only had space for red, green, and blue values per pixel, yet when I was setting pixel colors, I was passing a color that also contains an alpha value. This led to a segmentation fault at some point in the process, because I would be writing past the end of the memory allotted for the collage image's pixel data. That was the interesting issue. I solved it (and was pretty proud about that fact) but it still didn't work. Long story short, I forgot that "rows" means the Y value, and "columns" means the X value, and spent way more time than I should've trying all sorts of things when really I just needed to check if I had my function arguments in the correct order. Once I fixed that, the collage function worked for uniform images. Now I just need to figure out a consistent way to crop and scale images so I can make a collage with any collection of images I want. Then I'll figure out the layering and color logic to make the mosaic.

later. Got the mosaic filter thing to work. Wow. Now want to make it so I can more easily do it. rn I have to do the math manually: okay I have 10,000 100x100px tiles, which means my collage will be 100x100 tiles, and the image itself will be 10,000 pixels. And then the main image needs to be the same pixel dimensions as the collage's tile dimensions. i.e. in this case, main image would need to be 100x100 pixels. So each tile is mapped to a pixel in the main image, and then a blended color layer filter thing is applied.

This filter is just the process of taking a pixel of the main image and blending that pixel color with each pixel in the tile.

-----------------------
for each pixel in main image, take the tile image, and blend each pixel in the tile with the main image's given pixel.
Throw that tile into the collage