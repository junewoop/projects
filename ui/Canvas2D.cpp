/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"

#include <QCoreApplication>
#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "filter/FilterBlur.h"
#include "filter/FilterEdge.h"
#include "filter/FilterScale.h"

Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr),
    m_brushType(BRUSH_CONSTANT),
    m_brushColor(RGBA()),
    m_currentRadius(0),
    m_brush(nullptr),
    m_p(0),
    m_blurRadius(0),
    m_scaleX(0.f),
    m_scaleY(0.f),
    m_raySetting()
{
    settingsChanged();
}

Canvas2D::~Canvas2D()
{
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);

}

void Canvas2D::settingsChanged() {
    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;
    // Brush
    int s_type = settings.brushType;
    RGBA s_color = settings.brushColor;
    int s_radius = settings.brushRadius;
    if (s_type != m_brushType)
        std::cout << "  BrushType has changed" << std::endl;
    else if (s_radius != m_currentRadius)
        std::cout << "  BrushRadius has changed" << std::endl;
    else if ((s_color.r != m_brushColor.r) or
             (s_color.g != m_brushColor.g) or
             (s_color.b != m_brushColor.b) or
             (s_color.a != m_brushColor.a))
        std::cout << "  BrushColor has changed" << std::endl;
    m_brushType = s_type;
    m_brushColor = s_color;
    m_currentRadius = s_radius;
    switch(m_brushType) {
    case BRUSH_CONSTANT:
        m_brush = std::make_unique<ConstantBrush>(m_brushColor, m_currentRadius);
        break;
    case BRUSH_LINEAR:
        m_brush = std::make_unique<LinearBrush>(m_brushColor, m_currentRadius);
        break;
    case BRUSH_QUADRATIC:
        m_brush = std::make_unique<QuadraticBrush>(m_brushColor, m_currentRadius);
        break;
    case BRUSH_SMUDGE:
        m_brush = std::make_unique<SmudgeBrush>(m_brushColor, m_currentRadius);
        break;
    }

    // Filter
    m_p = settings.edgeDetectSensitivity;
    m_blurRadius = settings.blurRadius;
    m_scaleX = settings.scaleX;
    m_scaleY = settings.scaleY;

    // Ray
    m_raySetting = raySetting(settings.useShadows, settings.useTextureMapping, settings.useReflection,
                   settings.reflectionDepth, settings.useMultiThreading, settings.usePointLights,
                              settings.useDirectionalLights, settings.useKDTree);
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    std::cout << "Canvas2d::mouseDown() called" << std::endl;
    m_brush->brushDown(x, y, this);
//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

}

void Canvas2D::mouseDragged(int x, int y) {
    std::cout << "Canvas2d::mouseDragged() called" << std::endl;
    m_brush->brushDragged(x, y, this);

}

void Canvas2D::mouseUp(int x, int y) {
    std::cout << "Canvas2d::mouseUp() called" << std::endl;
    m_brush->brushUp(x, y, this);
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    std::unique_ptr<Filter> filter;
    switch(settings.filterType) {
    case FILTER_BLUR:
        filter = std::make_unique<FilterBlur>(m_blurRadius);
        filter->apply(this);
        break;
    case FILTER_EDGE_DETECT:
        filter = std::make_unique<FilterEdge>(m_p);
        filter->apply(this);
        break;
    case FILTER_SCALE:
        filter = std::make_unique<FilterScale>(m_scaleX, m_scaleY);
        filter->apply(this);
        break;
    }
    this->update();
    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);
}

void Canvas2D::renderImage(Camera *camera, int width, int height) {
    if (m_rayScene) {
        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.
        resize(width, height);
        m_rayScene->draw(this, camera, m_raySetting);
    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}
