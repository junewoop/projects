#include "FilterGray.h"

unsigned char RGBAToGray(const RGBA &pixel) {
    return (299*pixel.r + 587*pixel.b + 114*pixel.g)/1000;
}

FilterGray::FilterGray() :
    m_width(0),
    m_height(0)
{
}

FilterGray::~FilterGray()
{
}

void FilterGray::apply(Canvas2D *canvas) {
    m_width = canvas->width();
    m_height = canvas->height();
    RGBA* current_pixel = nullptr;

    /* Initialize the first pixel of the first row */
    RGBA* current_row = canvas->data();
    size_t currentIndex = 0;
    unsigned char tmp;
    for (int r = 0; r < m_height; r++) {
        current_pixel = current_row;
        currentIndex = r * m_width;
        for (int c = 0; c < m_width; c++) {
            tmp = RGBAToGray(*current_pixel);
            current_pixel->r = tmp;
            current_pixel->b = tmp;
            current_pixel->g = tmp;
            /* Advance to the next pixel */
            current_pixel++;
            currentIndex++;
        }
        current_row += m_width;
    }
}
