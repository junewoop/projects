#ifndef FILTERBLUR_H
#define FILTERBLUR_H
#include "Filter.h"

class FilterBlur : public Filter {
public:
    FilterBlur(int blurRadius);
    virtual ~FilterBlur() override;

    virtual void apply(Canvas2D *canvas) override;
private:
    void blur1D(int beginIndex, std::vector<float> &vec_r,std::vector<float> &vec_g, std::vector<float> &vec_b,
                std::vector<float> &next_r, std::vector<float> &next_g, std::vector<float> &next_b, AxisType axis);
    int m_r;
    int m_width;
    int m_height;
    float m_ker;
};
#endif // FILTERBLUR_H
