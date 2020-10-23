#ifndef FILTERSCALE_H
#define FILTERSCALE_H
#include "Filter.h"

inline float backmap(float scale, int j){
        return j/scale + (1-scale)/scale;
}

inline float trianglefilter(float diff, float scale){
    float radius;
    if (scale < 1)
        radius = 1.f/scale;
    else
        radius = 1.f;
    if ((diff < -radius) || (diff > radius))
        return 0;
    else
        return (1-fabs(diff)/radius)/radius;
}

class FilterScale : public Filter {
public:
    FilterScale(float scaleX, float scaleY);
    virtual ~FilterScale() override;

    virtual void apply(Canvas2D *canvas) override;
private:
    void scale1D(int beginIndex, int old_beginIndex, std::vector<float> &vec_r,std::vector<float> &vec_g, std::vector<float> &vec_b,
                     std::vector<float> &next_r, std::vector<float> &next_g, std::vector<float> &next_b, AxisType axis);
    float m_scaleX;
    float m_scaleY;
    int m_old_width;
    int m_old_height;
    int m_new_width;
    int m_new_height;
};

#endif // FILTERSCALE_H
