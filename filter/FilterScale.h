#ifndef FILTERSCALE_H
#define FILTERSCALE_H
#include "Filter.h"

class FilterScale : public Filter {
public:
    FilterScale(float scaleX, float scaleY);
    ~FilterScale() override;
    void apply(Canvas2D *canvas) override;

private:
    float backmap(float scale, int j);
    float trianglefilter(float diff, float scale);
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
