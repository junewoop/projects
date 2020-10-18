#include "FilterUtils.h"
#include <algorithm>
#include "math.h"
#include <cstring>

namespace FilterUtils {

inline unsigned char REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

void Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel) {
    // TODO: Task 9 Create buffer to store new image data
    std::vector<RGBA> result;
    result.resize(width*height);
    // TODO: Task 10 Obtain kernel dimension
    int k_dim = sqrt(kernel.size());
    int k_r = (k_dim-1)/2;
    int r_tmp, c_tmp, ind_canvas, ind_ker;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;

//            // TODO: Task 11 Initialize color to accumulate convolution data
             float red_acc = 0.f;
             float green_acc = 0.f;
             float blue_acc = 0.f;
            // TODO: Task 12
//            // Iterate over the kernel using the value from task 10
//            // Obtain the value at current index of kernel
//            // Find correct index in original image data
//            // Accumulate the kernel applied to pixel value in color_acc

//            // TODO: Task 13 Ignore outside bounds
            for (int i = -k_r; i <= k_r; i ++){
                for (int j = -k_r; j <= k_r; j++){
                    r_tmp = r + i;
                    c_tmp = c + j;
                    if (r_tmp >= 0 and r_tmp < height and c_tmp >= 0 and c_tmp < width){
                        ind_canvas = r_tmp*width+c_tmp;
                        ind_ker = (i+k_r)*k_dim + (j + k_r);
                        red_acc += data[ind_canvas].r/255.f * kernel[ind_ker];
                        green_acc += data[ind_canvas].g/255.f * kernel[ind_ker];
                        blue_acc += data[ind_canvas].b/255.f * kernel[ind_ker];
                    }
                    else continue;
                }
            }
            // TODO: Task 14 Update buffer with accumulated color
            result[centerIndex].r = REAL2byte(red_acc);
            result[centerIndex].g = REAL2byte(green_acc);
            result[centerIndex].b = REAL2byte(blue_acc);
            result[centerIndex].a = data[centerIndex].a;
        }
    }

    // TODO: Task 15 Copy over buffer to canvas data
    memcpy(data, &result[0], width*height*sizeof(RGBA));
}

}
