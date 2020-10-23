#include "Filter.h"

Filter::~Filter()
{
}

void Filter::loadImage(Canvas2D *canvas, std::vector<float> &vec_r, std::vector<float> &vec_g, std::vector<float> &vec_b){
    int width = canvas->width();
    int height = canvas->height();
    RGBA *data = canvas->data();
    vec_r.resize(width*height);
    vec_g.resize(width*height);
    vec_b.resize(width*height);
    int ind = 0;
    for (int r = 0; r < height; r++){
        for (int c = 0; c < width; c++){
            vec_r[ind] = data[ind].r/255.f;
            vec_g[ind] = data[ind].g/255.f;
            vec_b[ind] = data[ind].b/255.f;
            ind++;
        }
    }
}

void Filter::saveImage(Canvas2D *canvas, std::vector<float> &vec_r, std::vector<float> &vec_g, std::vector<float> &vec_b){
    int width = canvas->width();
    int height = canvas->height();
    RGBA *data = canvas->data();
    int ind = 0;
    for (int r = 0; r< height; r++){
        for (int c = 0; c < width; c++){
            data[ind].r = REAL2byte(vec_r[ind]);
            data[ind].g = REAL2byte(vec_g[ind]);
            data[ind].b = REAL2byte(vec_b[ind]);
            data[ind].a = 255;
            ind++;
        }
    }
}
