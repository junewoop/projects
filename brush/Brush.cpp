/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"

unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}

Brush::Brush(RGBA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_radius(radius),
    m_maskSize(2*radius+1)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

RGBA Brush::getRGBA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setRGBA(const RGBA &rgba) {
    m_color = rgba;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    makeMask();
}




void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.

    RGBA *pix = canvas->data();
    int width = canvas->width();
    int height = canvas->height();
    int beginRow = std::max(0, mouseY-m_radius);
    int endRow = std::min(height, mouseY+m_radius+1);
    int beginCol = std::max(0, mouseX-m_radius);
    int endCol = std::min(width, mouseX+m_radius+1);
    unsigned char r, g, b, a = 255;
    int indCanvas;
    int indBrush;
    for (int i = beginRow; i < endRow; i++)
        for (int j = beginCol; j < endCol; j++){
            indCanvas = i*width+j;
            indBrush = getIndex(i-mouseY+m_radius, j-mouseX+m_radius);
            r = lerp(pix[indCanvas].r, m_color.r, m_mask[indBrush]);
            g = lerp(pix[indCanvas].g, m_color.g, m_mask[indBrush]);
            b = lerp(pix[indCanvas].b, m_color.b, m_mask[indBrush]);
            pix[indCanvas] = RGBA(r, g, b, a);
        }
    canvas->update();
}

int Brush::squaredDistance(int i, int j) {
    // From 2D index, compute the squared distance from the center of the mask.
    int diffX = j - m_radius;
    int diffY = i - m_radius;
    return diffX*diffX + diffY*diffY;
}

int Brush::getIndex(int i, int j) {
    // Converts to 2D index into the index of m_mask
    return i*m_maskSize+j;
}
