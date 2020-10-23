#include "FilterEdge.h"

FilterEdge::FilterEdge(float p) :
    m_p(p)
{
}

FilterEdge::~FilterEdge()
{
}

void FilterEdge::apply(Canvas2D *canvas){
    FilterGray::apply(canvas);
    int width = canvas->width(),height = canvas->height();
    RGBA* data = canvas->data();
    std::vector<int> G_x_int;
    std::vector<int> G_y_int;
    int ind = 0;
    float g_x, g_y;
    G_x_int.resize(width*height);
    G_y_int.resize(width*height);
    for (int r = 0; r < height; r++){
        for (int c = 0; c < width; c++){
            if(c == 0){
                G_x_int[ind] = -data[ind].r + data[ind + 1].r;
                G_y_int[ind] = 2* data[ind].r + data[ind + 1].r;
            }
            else if (c == width - 1){
                G_x_int[ind] = -data[ind-1].r + data[ind].r;
                G_y_int[ind] = data[ind-1].r + 2*data[ind].r;
            }
            else{
                G_x_int[ind] = -data[ind-1].r + data[ind+1].r;
                G_y_int[ind] = data[ind-1].r + 2*data[ind].r + data[ind + 1].r;
            }
            ind++;
        }
    }
    ind = 0;
    for (int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            if(r == 0){
                g_x = 2*G_x_int[ind] + G_x_int[ind+width];
                g_y = G_y_int[ind] - G_y_int[ind+width];
            }
            else if (r == height - 1){
                g_x = G_x_int[ind-width] + 2*G_x_int[ind];
                g_y = -G_y_int[ind-width] + G_y_int[ind];
            }
            else{
                g_x = G_x_int[ind-width] + 2*G_x_int[ind] + G_x_int[ind + width];
                g_y = -G_y_int[ind-width] + G_y_int[ind+width];
            }
            data[ind].r = REAL2byte(sqrtf(g_x*g_x+g_y*g_y)/255.f*m_p);
            data[ind].g = REAL2byte(sqrtf(g_x*g_x+g_y*g_y)/255.f*m_p);
            data[ind].b = REAL2byte(sqrtf(g_x*g_x+g_y*g_y)/255.f*m_p);
            ind++;
        }
    }
}
