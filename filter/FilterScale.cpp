#include "FilterScale.h"

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

    // load the original image
    std::vector<float> vec_r, vec_g, vec_b;
    loadImage(canvas, vec_r, vec_g, vec_b);

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
    saveImage(canvas, final_r, final_g, final_b);
    data = canvas->data();
}

float FilterScale::backmap(float scale, int j){
        return j/scale + (1-scale)/scale;
}

float FilterScale::trianglefilter(float diff, float scale){
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

void FilterScale::scale1D(int beginIndex, int old_beginIndex,
                              std::vector<float> &vec_r,std::vector<float> &vec_g, std::vector<float> &vec_b,
                              std::vector<float> &next_r, std::vector<float> &next_g, std::vector<float> &next_b,
                              AxisType axis){
    // initialization with respect to axis
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

    // begin applying filter
    int ind = 0;
    int begin, end;
    float r, g, b;
    float weight_sum = 0.f;
    for (int j = 0; j < step; j++){
        // initialize pixels
        ind = shiftIndex(beginIndex, j, m_new_width, axis);
        next_r[ind] = 0.f;
        next_g[ind] = 0.f;
        next_b[ind] = 0.f;
        // compute backmap and support range
        center = backmap(scale, j);
        if (scale < 1){
            begin = ceil(center - 1/scale);
            end = floor(center + 1/scale);
        }
        else{
            begin = ceil(center - 1);
            end = ceil(center);
        }
        // compute convolution
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
        // normalize: divide by weight_sum
        next_r[ind] /= weight_sum;
        next_g[ind] /= weight_sum;
        next_b[ind] /= weight_sum;
    }
}

