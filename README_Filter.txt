0. Design Decisions
 I have five filter classes, namely Filter, FilterGray, FilterEdge, FilterBlur, and FilterScale. 
 Filter is a superclass of FilterGray, FilterBlur, and FilterSclae. FilterEdge inherits from
 FilterGray. I implemented fast box filter in FilterBlur and triangle filter with appropriate
 support (namely, 2/scale if scale < 1 and 2 otherwise) for a kernel that approximates sinc 
 in FilterScale. All extended pixels past the boundary of original images take the pixel values
 at the nearest edges.

 Filter has two methods, namely loadImage() and saveImage() that loads images from
 and save images to canvas. Filter.h also contains inline functions REAL2byte() and shiftINdex().
 REAL2byte() converts from real numbers in range [0, 1] to unsigned char. shiftIndex() shifts index
 by given offset with respect to the given axis. 

 The rest of the classes have apply() method that applies its filter to the image. FilterGray::apply()
 is exactly the analogous function in Filter lab. FilterEdge::apply() first applies its superclass apply(),
 namely FilterGray::apply(), to the image, and applies the sobel kernels. FilterBlur::apply() uses
 FilterBlur::blur1D() that can be used to apply 1-dimensional box blur to one row/column. FilterScale::apply()
 uses FilterScale::scale1D() that can be used to apply 1-dimensional scale filter to one row/column.
 It also uses backmap() and tiranglefilter() to do repetitive comptuation.

 1. Fast Box Filter for FilterBlur
 I implemented faxt box filter for the blur filter. First, box filter can be decomposed into two 
 1-dimensional averaging filters. Since each weight in the kernel is identical, each filtered
 pixel can be computed by subracting the leftmost weighted pixel added in the computation of previous
 filtered pixel and adding the new rightmost weighted pixel. Hence, computing a filtered row takes
 O(r + width) time since O(r) time is needed for computing the first pixel and O(width) time is needed
 for computing the rest by subtracting once and adding once for each pixel. Hence, it takes
 O(height(r + width)) time for applying horizontal 1-dimensional box blur kernel to the whole image.
 Similarly, O(width(r+height)) time is needed for vertical 1-dimensional box blur kernel.
 Therefore, it takes O(width*height + r(width+height)) time complexity for the whole image.
 Assuming that r is smaller than width or height, this is just O(width*height), so it takes in average
 O(1) time to compute one filtered pixel.
