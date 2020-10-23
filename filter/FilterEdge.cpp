#include "FilterEdge.h"

FilterEdge::FilterEdge(float p) :
    m_p(p)
{
}

FilterEdge::~FilterEdge()
{
}

void FilterEdge::apply(Canvas2D *canvas){
    // apply Grayscale
    FilterGray::apply(canvas);

    // load image
    std::vector<float> vec_gray, tmp_g_x, tmp_g_y;
    loadImage(canvas, vec_gray, tmp_g_x, tmp_g_y);

    // apply horizontal 1D kernel
    int ind = 0;
    for (int r = 0; r < m_height; r++){
        for (int c = 0; c < m_width; c++){
            if(c == 0){
                tmp_g_x[ind] = -vec_gray[ind] + vec_gray[ind + 1];
                tmp_g_y[ind] = 2* vec_gray[ind] + vec_gray[ind + 1];
            }
            else if (c == m_width - 1){
                tmp_g_x[ind] = -vec_gray[ind-1] + vec_gray[ind];
                tmp_g_y[ind] = vec_gray[ind-1] + 2*vec_gray[ind];
            }
            else{
                tmp_g_x[ind] = -vec_gray[ind-1] + vec_gray[ind+1];
                tmp_g_y[ind] = vec_gray[ind-1] + 2*vec_gray[ind] + vec_gray[ind + 1];
            }
            ind++;
        }
    }

    // apply vertical 1D kernel
    ind = 0;
    float g_x = 0.f, g_y = 0.f;
    for (int r = 0; r < m_height; r++){
        for(int c = 0; c < m_width; c++){
            if(r == 0){
                g_x = 2*tmp_g_x[ind] + tmp_g_x[ind+m_width];
                g_y = tmp_g_y[ind] - tmp_g_y[ind+m_width];
            }
            else if (r == m_height - 1){
                g_x = tmp_g_x[ind-m_width] + 2*tmp_g_x[ind];
                g_y = -tmp_g_y[ind-m_width] + tmp_g_y[ind];
            }
            else{
                g_x = tmp_g_x[ind-m_width] + 2*tmp_g_x[ind] + tmp_g_x[ind + m_width];
                g_y = -tmp_g_y[ind-m_width] + tmp_g_y[ind+m_width];
            }
            vec_gray[ind] = sqrtf(g_x*g_x+g_y*g_y)*m_p;
            ind++;
        }
    }

    // save image
    saveImage(canvas, vec_gray, vec_gray, vec_gray);
}
