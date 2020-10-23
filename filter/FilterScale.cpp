#include "FilterScale.h"
#include <iostream>

FilterScale::FilterScale(float scaleX, float scaleY) :
    m_scaleX(scaleX),
    m_scaleY(scaleY),
    m_old_width(0),
    m_old_height(0),
    m_new_width(0),
    m_new_height(0)
{
}

FilterScale::~FilterScale()
{
}

void FilterScale::apply(Canvas2D *canvas){
    // get data and canvas sizes
    RGBA *data = canvas->data();
    m_old_width = canvas->width();
    m_old_height = canvas->height();
    m_new_width = std::max(1, static_cast<int>(m_old_width*m_scaleX));
    m_new_height = std::max(1, static_cast<int>(m_old_height*m_scaleY));
    // Store the original image
    std::vector<float> vec_r, vec_g, vec_b;
    vec_r.resize(m_old_width*m_old_height);
    vec_g.resize(m_old_width*m_old_height);
    vec_b.resize(m_old_width*m_old_height);
    int ind = 0;
    for (int r = 0; r < m_old_height; r++){
        for (int c = 0; c < m_old_width; c++){
            vec_r[ind] = data[ind].r/255.f;
            vec_g[ind] = data[ind].g/255.f;
            vec_b[ind] = data[ind].b/255.f;
            ind++;
        }
    }
    // x-scaling
    std::vector<float> tmp_r, tmp_g, tmp_b;
    tmp_r.resize(m_new_width*m_old_height);
    tmp_g.resize(m_new_width*m_old_height);
    tmp_b.resize(m_new_width*m_old_height);
    for (int r = 0; r < m_old_height; r++)
        scale1D(m_new_width*r, m_old_width*r, vec_r, vec_g, vec_b, tmp_r, tmp_g, tmp_b, X_Axis);
    // y-scaling
    m_old_width = m_new_width;
    std::vector<float> final_r, final_g, final_b;
    final_r.resize(m_old_width*m_new_height);
    final_g.resize(m_old_width*m_new_height);
    final_b.resize(m_old_width*m_new_height);
    for (int c = 0; c < m_old_width; c++)
        scale1D(c, c, tmp_r, tmp_g, tmp_b, final_r, final_g, final_b, Y_Axis);
    // resize and store new image
    canvas->resize(m_new_width, m_new_height);
    data = canvas->data();
    ind = 0;
    for (int r = 0; r < m_new_height; r++){
        for (int c = 0; c < m_new_width; c++){
            data[ind].r = REAL2byte(final_r[ind]);
            data[ind].g = REAL2byte(final_g[ind]);
            data[ind].b = REAL2byte(final_b[ind]);
            data[ind].a = 255;
            ind++;
        }
    }
}

void FilterScale::scaleUp1D(int beginIndex,int old_beginIndex,
                            std::vector<float> &vec_r,std::vector<float> &vec_g, std::vector<float> &vec_b,
                            std::vector<float> &next_r, std::vector<float> &next_g, std::vector<float> &next_b,
                            AxisType axis){
    int step = 0;
    int old_step = 0;
    float scale = 0.f;
    float center = 0.f;
    switch(axis){
    case X_Axis:
        step = m_new_width;
        old_step = m_old_width;
        scale = m_scaleX;
        break;
    case Y_Axis:
        step = m_new_height;
        old_step = m_old_height;
        scale = m_scaleY;
        break;
    }
    int ind = 0;
    int left = 0, right = 0;
    float left_r = 0.f, left_g = 0.f, left_b = 0.f;
    float right_r = 0.f, right_g = 0.f, right_b = 0.f;
    for (int j = 0; j < step; j++){
        center = backmap(scale, j);
        right = ceil(center);
        left = right - 1;
        if (left < 0){
            left_r = vec_r[old_beginIndex];
            left_g = vec_g[old_beginIndex];
            left_b = vec_b[old_beginIndex];
        }
        else{
            left_r = vec_r[shiftIndex(old_beginIndex, left, m_old_width, axis)];
            left_g = vec_g[shiftIndex(old_beginIndex, left, m_old_width, axis)];
            left_b = vec_b[shiftIndex(old_beginIndex, left, m_old_width, axis)];
        }
        if (right >= old_step){
            right_r = vec_r[shiftIndex(old_beginIndex, old_step-1, m_old_width, axis)];
            right_g = vec_g[shiftIndex(old_beginIndex, old_step-1, m_old_width, axis)];
            right_b = vec_b[shiftIndex(old_beginIndex, old_step-1, m_old_width, axis)];
        }
        else{
            right_r = vec_r[shiftIndex(old_beginIndex, right, m_old_width, axis)];
            right_g = vec_g[shiftIndex(old_beginIndex, right, m_old_width, axis)];
            right_b = vec_b[shiftIndex(old_beginIndex, right, m_old_width, axis)];
        }
        ind = shiftIndex(beginIndex, j, m_new_width, axis);
        next_r[ind] = left_r * trianglefilter(left - center, scale) + right_r * trianglefilter(right - center, scale);
        next_g[ind] = left_g * trianglefilter(left - center, scale) + right_g * trianglefilter(right - center, scale);
        next_b[ind] = left_b * trianglefilter(left - center, scale) + right_b * trianglefilter(right - center, scale);
    }
}

void FilterScale::scale1D(int beginIndex, int old_beginIndex,
                              std::vector<float> &vec_r,std::vector<float> &vec_g, std::vector<float> &vec_b,
                              std::vector<float> &next_r, std::vector<float> &next_g, std::vector<float> &next_b,
                              AxisType axis){
    int step = 0;
    int old_step = 0;
    float scale = 0.f;
    float center = 0.f;
    switch(axis){
    case X_Axis:
        step = m_new_width;
        old_step = m_old_width;
        scale = m_scaleX;
        break;
    case Y_Axis:
        step = m_new_height;
        old_step = m_old_height;
        scale = m_scaleY;
        break;
    }
    int ind = 0;
    int begin, end;
    float r, g, b;
    float weight_sum = 0.f;
    for (int j = 0; j < step; j++){
        ind = shiftIndex(beginIndex, j, m_new_width, axis);
        next_r[ind] = 0.f;
        next_g[ind] = 0.f;
        next_b[ind] = 0.f;
        center = backmap(scale, j);
        begin = ceil(center - 1/scale);
        end = floor(center + 1/scale);
        weight_sum = 0.f;
        for(int k = begin; k <= end; k++){
            if (k < 0){
                r = vec_r[old_beginIndex];
                g = vec_g[old_beginIndex];
                b = vec_b[old_beginIndex];
            }
            else if(k >= old_step){
                r = vec_r[shiftIndex(old_beginIndex, old_step-1, m_old_width, axis)];
                g = vec_g[shiftIndex(old_beginIndex, old_step-1, m_old_width, axis)];
                b = vec_b[shiftIndex(old_beginIndex, old_step-1, m_old_width, axis)];
            }
            else{
                r = vec_r[shiftIndex(old_beginIndex, k, m_old_width, axis)];
                g = vec_g[shiftIndex(old_beginIndex, k, m_old_width, axis)];
                b = vec_b[shiftIndex(old_beginIndex, k, m_old_width, axis)];
            }
            next_r[ind] += r * trianglefilter(k - center, scale);
            next_g[ind] += g * trianglefilter(k - center, scale);
            next_b[ind] += b * trianglefilter(k - center, scale);
            weight_sum += trianglefilter(k - center, scale);
        }
        next_r[ind] /= weight_sum;
        next_g[ind] /= weight_sum;
        next_b[ind] /= weight_sum;
    }
}

int FilterScale::shiftIndex(int ind, int offset, int width, AxisType axis){
    switch(axis){
    case X_Axis:
        return ind + offset;
    case Y_Axis:
        return ind + offset * width;
    }
}
