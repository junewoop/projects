#include "FilterBlur.h"

FilterBlur::FilterBlur(int blurRadius) :
 m_r(blurRadius),
  m_width(0),
  m_height(0),
  m_ker(0.f)
{
}

FilterBlur::~FilterBlur(){

}

void FilterBlur::apply(Canvas2D *canvas){
    // initialize member variables
    m_width = canvas->width();
    m_height = canvas->height();
    m_ker = 1.f/static_cast<float>(2*m_r+1);

    // load image
    std::vector<float> vec_r, vec_g, vec_b;
    loadImage(canvas, vec_r, vec_g, vec_b);

    //TODO: 1D horizontal kernel
    std::vector<float> tmp_r, tmp_g, tmp_b;
    tmp_r.resize(m_width*m_height);
    tmp_g.resize(m_width*m_height);
    tmp_b.resize(m_width*m_height);
    for (int r = 0; r < m_height; r++)
        blur1D(r*m_width, vec_r, vec_g, vec_b, tmp_r, tmp_g, tmp_b, X_Axis);

    //TODO: 1D vertical kernel
    std::vector<float> final_r, final_g, final_b;
    final_r.resize(m_width*m_height);
    final_g.resize(m_width*m_height);
    final_b.resize(m_width*m_height);
    //for (int c = 0; c < m_width; c++)
    //    blur1D(c, tmp_r, tmp_g, tmp_b, final_r, final_g, final_b, Y_Axis);

    //save Image
    saveImage(canvas, tmp_r, tmp_g, tmp_b);
}

void FilterBlur::blur1D(int beginIndex,
                        std::vector<float> &vec_r, std::vector<float> &vec_g, std::vector<float> &vec_b,
                        std::vector<float> &next_r, std::vector<float> &next_g, std::vector<float> &next_b,
                        AxisType axis){
    int ind = beginIndex;
    int num_step = 0;
    switch(axis){
    case X_Axis:
        num_step = m_width;
        break;
    case Y_Axis:
        num_step = m_height;
    }

    int endIndex = shiftIndex(beginIndex, num_step-1, m_width, axis);
    next_r[beginIndex] = m_r * vec_r[beginIndex] * m_ker;
    next_g[beginIndex] = m_r * vec_g[beginIndex] * m_ker;
    next_b[beginIndex] = m_r * vec_b[beginIndex] * m_ker;
    for (int j = 0; j <= m_r; j++){
        if(j < num_step){
            ind = shiftIndex(beginIndex, j, m_width, axis);
            next_r[beginIndex] += vec_r[ind]*m_ker;
            next_g[beginIndex] += vec_g[ind]*m_ker;
            next_b[beginIndex] += vec_b[ind]*m_ker;
        }
        else{
            next_r[beginIndex] += vec_r[endIndex]*m_ker;
            next_g[beginIndex] += vec_g[endIndex]*m_ker;
            next_b[beginIndex] += vec_b[endIndex]*m_ker;
        }
    }

    int prev_ind, tmp_ind;
    for (int j = 1; j < num_step; j++){
        prev_ind = shiftIndex(beginIndex, j-1, m_width, axis);
        ind = shiftIndex(beginIndex, j, m_width, axis);
        if(j-1-m_r < 0){
            next_r[ind] = next_r[prev_ind] - vec_r[beginIndex]*m_ker;
            next_g[ind] = next_g[prev_ind] - vec_g[beginIndex]*m_ker;
            next_b[ind] = next_b[prev_ind] - vec_b[beginIndex]*m_ker;
        }
        else{
            tmp_ind = shiftIndex(beginIndex, j-1-m_r, m_width, axis);
            next_r[ind] = next_r[prev_ind] - vec_r[tmp_ind]*m_ker;
            next_g[ind] = next_g[prev_ind] - vec_g[tmp_ind]*m_ker;
            next_b[ind] = next_b[prev_ind] - vec_b[tmp_ind]*m_ker;
        }
        if(j+m_r >= num_step){
            next_r[ind] = next_r[prev_ind] + vec_r[endIndex]*m_ker;
            next_g[ind] = next_g[prev_ind] + vec_g[endIndex]*m_ker;
            next_b[ind] = next_b[prev_ind] + vec_b[endIndex]*m_ker;
        }
        else{
            tmp_ind = shiftIndex(beginIndex, j+m_r, m_width, axis);
            next_r[ind] = next_r[prev_ind] + vec_r[tmp_ind]*m_ker;
            next_g[ind] = next_g[prev_ind] + vec_g[tmp_ind]*m_ker;
            next_b[ind] = next_b[prev_ind] + vec_b[tmp_ind]*m_ker;
        }
    }
}


