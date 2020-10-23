#ifndef FILTER_H
#define FILTER_H

#include <assert.h>
#include "RGBA.h"
#include "Canvas2D.h"

enum AxisType {
    X_Axis,
    Y_Axis
};

class Canvas2D;

/**
 * @class Filter
 *
 * This is the superclass for all filters.
 *
 * It declares the abstract method apply, which is called by Canvas2D to apply the filter.
 */

inline unsigned char REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

inline int shiftIndex(int ind, int offset, int width, AxisType axis){
    switch(axis){
    case X_Axis:
        return ind + offset;
    case Y_Axis:
        return ind + offset * width;
    }
}

class Filter {

public:
    virtual ~Filter();

    // Applies this filter on the given region.
    //
    // The apply method is called by FilterCanvas to apply this filter
    // to an entire image. This method is abstract, you will need to
    // redefine it each filter subclass.
    //
    virtual void apply(Canvas2D* canvas) = 0;
protected:
    void loadImage(Canvas2D* canvas, std::vector<float> &vec_r, std::vector<float> &vec_g, std::vector<float> &vec_b);
    void saveImage(Canvas2D* canvas, std::vector<float> &vec_r, std::vector<float> &vec_g, std::vector<float> &vec_b);
};

#endif

