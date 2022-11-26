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