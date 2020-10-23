#include "FilterBlur.h"
#include "FilterUtils.h"

FilterBlur::FilterBlur(int blurRadius) :
 m_r(blurRadius)
{
}

FilterBlur::~FilterBlur(){

}

void FilterBlur::apply(Canvas2D *canvas){
    //TODO: Edge cases
    int width = canvas->width(), height = canvas->height();
    int canvas_size = width*height;
    RGBA *data = canvas->data();
    float ker = 1.f/static_cast<float>(2*m_r+1);
    int ind = 0;
    std::vector<float> vec_r, vec_g, vec_b;
    vec_r.resize(canvas_size);
    vec_g.resize(canvas_size);
    vec_b.resize(canvas_size);
    for (int r = 0; r < height; r++){
        ind = r*width;
        vec_r[r*width] = m_r * data[r*width].r/255.f*ker;
        vec_g[r*width] = m_r * data[r*width].g/255.f*ker;
        vec_b[r*width] = m_r * data[r*width].b/255.f*ker;
        for (int c = 0; c <= m_r; c++){
            vec_r[r*width] += data[ind].r/255.f*ker;
            vec_g[r*width] += data[ind].g/255.f*ker;
            vec_b[r*width] += data[ind].b/255.f*ker;
            ind++;
        }
        ind = r * width + 1;
        for (int c = 1; c < m_r+1; c++){
            vec_r[ind] = vec_r[ind-1] - data[r*width].r/255.f*ker + data[ind + m_r].r/255.f*ker;
            vec_g[ind] = vec_g[ind-1] - data[r*width].g/255.f*ker + data[ind + m_r].g/255.f*ker;
            vec_b[ind] = vec_b[ind-1] - data[r*width].b/255.f*ker + data[ind + m_r].b/255.f*ker;
            ind++;
        }
        for (int c = m_r+1;c < width - m_r; c++){
            vec_r[ind] = vec_r[ind-1] - data[ind-1-m_r].r/255.f*ker + data[ind+m_r].r/255.f*ker;
            vec_g[ind] = vec_g[ind-1] - data[ind-1-m_r].g/255.f*ker + data[ind+m_r].g/255.f*ker;
            vec_b[ind] = vec_b[ind-1] - data[ind-1-m_r].b/255.f*ker + data[ind+m_r].b/255.f*ker;
            ind++;
        }
        for (int c = width - m_r; c<width; c++){
            vec_r[ind] = vec_r[ind-1] - data[ind-1-m_r].r/255.f*ker + data[(r+1)*width-1].r/255.f*ker;
            vec_g[ind] = vec_g[ind-1] - data[ind-1-m_r].g/255.f*ker + data[(r+1)*width-1].g/255.f*ker;
            vec_b[ind] = vec_b[ind-1] - data[ind-1-m_r].b/255.f*ker + data[(r+1)*width-1].b/255.f*ker;
            ind++;
        }
    }
    std::vector<float> data_r, data_g, data_b;
    data_r.resize(canvas_size);
    data_g.resize(canvas_size);
    data_b.resize(canvas_size);
    for (int c= 0; c < width; c++){
        data_r[c] = m_r*vec_r[c]*ker;
        data_g[c] = m_r*vec_g[c]*ker;
        data_b[c] = m_r*vec_b[c]*ker;
        ind = c;
        for (int r = 0; r <= m_r; r++){
            data_r[c] += vec_r[ind]*ker;
            data_g[c] += vec_g[ind]*ker;
            data_b[c] += vec_b[ind]*ker;
            ind += width;
        }
        ind = c + width;
        for (int r = 1; r < m_r + 1; r++){
            data_r[ind] = data_r[ind - width] - vec_r[c]*ker + vec_r[ind + m_r*width]*ker;
            data_g[ind] = data_g[ind - width] - vec_g[c]*ker + vec_g[ind + m_r*width]*ker;
            data_b[ind] = data_b[ind - width] - vec_b[c]*ker + vec_b[ind + m_r*width]*ker;
            ind += width;
        }
        for (int r = m_r+1; r < height - m_r; r++){
            data_r[ind] = data_r[ind-width] - vec_r[ind-(1+m_r)*width]*ker + vec_r[ind + m_r*width]*ker;
            data_g[ind] = data_g[ind-width] - vec_g[ind-(1+m_r)*width]*ker + vec_g[ind + m_r*width]*ker;
            data_b[ind] = data_b[ind-width] - vec_b[ind-(1+m_r)*width]*ker + vec_b[ind + m_r*width]*ker;
            ind += width;
        }
        for (int r = height - m_r; r < height; r++){
            data_r[ind] = data_r[ind-width] - vec_r[ind - (1+m_r)*width]*ker + vec_r[c+(height-1)*width]*ker;
            data_g[ind] = data_g[ind-width] - vec_g[ind - (1+m_r)*width]*ker + vec_g[c+(height-1)*width]*ker;
            data_b[ind] = data_b[ind-width] - vec_b[ind - (1+m_r)*width]*ker + vec_b[c+(height-1)*width]*ker;
            ind += width;
        }
    }
    ind = 0;
    for (int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            data[ind].r = REAL2byte(data_r[ind]);
            data[ind].g = REAL2byte(data_g[ind]);
            data[ind].b = REAL2byte(data_b[ind]);
            ind++;
        }
    }
}
