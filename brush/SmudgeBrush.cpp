/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"


SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!

    makeMask();
    initializePaint();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.
}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {
    int width = canvas->width();
    int height = canvas->height();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //
    RGBA *pix = canvas->data();
    int beginRow = std::max(0, y-m_radius);
    int endRow = std::min(height, y+m_radius+1);
    int beginCol = std::max(0, x-m_radius);
    int endCol = std::min(width, x+m_radius+1);
    int indCanvas;
    int indBrush;
    for (int i = beginRow; i < endRow; i++)
        for (int j = beginCol; j < endCol; j++){
            indCanvas = i*width+j;
            indBrush = getIndex(i-y+m_radius, j-x+m_radius);
            m_paint[indBrush] = pix[indCanvas];
        }
}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

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
            r = lerp(pix[indCanvas].r, m_paint[indBrush].r, m_mask[indBrush]);
            g = lerp(pix[indCanvas].g, m_paint[indBrush].g, m_mask[indBrush]);
            b = lerp(pix[indCanvas].b, m_paint[indBrush].b, m_mask[indBrush]);
            pix[indCanvas] = RGBA(r, g, b, a);
        }
    canvas->update();
    // now pick up paint again...
    pickUpPaint(mouseX, mouseY, canvas);

}

void SmudgeBrush::initializePaint() {
    m_paint.reserve(m_maskSize*m_maskSize);
    for (int i = 0; i < m_maskSize; i++)
        for (int j = 0; j < m_maskSize; j++)
            m_paint[getIndex(i, j)] = RGBA();
}


