#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <memory>

#include "SupportCanvas2D.h"
#include "Brush.h"

class RayScene;

class Camera;

/**
 * @class Canvas2D
 *
 * 2D canvas that students will implement in the Brush and Filter assignments. The same canvas
 * will be used to display raytraced images in the Intersect and Ray assignments.
 */
class Canvas2D : public SupportCanvas2D {
    Q_OBJECT
public:
    Canvas2D();
    virtual ~Canvas2D();

    void setScene(RayScene *scene);

    // UI will call this from the button on the "Ray" dock
    void renderImage(Camera *camera, int width, int height);

    // This will be called when the settings have changed
    virtual void settingsChanged();

    QImage* getImage() { return m_image; }


public slots:
    // UI will call this from the button on the "Ray" dock
    void cancelRender();

    // UI will call this from the button on the "Filter" dock
    void filterImage();

protected:
    virtual void paintEvent(QPaintEvent *);  // Overridden from SupportCanvas2D.
    virtual void mouseDown(int x, int y);    // Called when left mouse button is pressed on canvas
    virtual void mouseDragged(int x, int y); // Called when left mouse button is dragged on canvas
    virtual void mouseUp(int x, int y);      // Called when left mouse button is released

    // Called when the size of the canvas has been changed
    virtual void notifySizeChanged(int w, int h);

private:
    std::unique_ptr<RayScene> m_rayScene;

    int m_brushType;
    RGBA m_brushColor;
    int m_currentRadius;
    std::unique_ptr<Brush> m_brush;

    float m_p;
    int m_blurRadius;
    float m_scaleX;
    float m_scaleY;


    bool m_useKDTree;
    bool m_useMultiThreading;
};

#endif // CANVAS2D_H
