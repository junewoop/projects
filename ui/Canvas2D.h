#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <memory>

#include "SupportCanvas2D.h"
#include "Brush.h"

class RayScene;

class Camera;

inline unsigned char REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

struct raySetting{
    raySetting() : useShadows(false), useTextureMapping(false), useReflection(false),
        reflectionDepth(0), useMultiThreading(false), usePointLights(false),
        useDirectionalLights(false), useKDTree(false){}
    raySetting(bool a, bool b, bool c, int d, bool e, bool f, bool g, bool h) :
        useShadows(a), useTextureMapping(b), useReflection(c),
        reflectionDepth(d), useMultiThreading(e), usePointLights(f),
        useDirectionalLights(g), useKDTree(h){}
// bool useSuperSampling;      // Enable or disable super-sampling.
// int numSuperSamples;        // Controls the number of samples per pixel.
// bool useAntiAliasing;       // Enable or disable anti-aliasing.
bool useShadows;               // Enable or disable shadows.
bool useTextureMapping;        // Enable or disable texture mapping.
bool useReflection;            // Enable or disable reflection.
int reflectionDepth;           // Depth of the reflection
// bool useRefraction;         // Enable or disable refraction (this is extra credit).
bool useMultiThreading;     // Enable or disable multi-threading (extra credit).
bool usePointLights;           // Enable or disable point lighting.
bool useDirectionalLights;     // Enable or disable directional lighting (extra credit).
// bool useSpotLights;         // Enable or disable spot lights (extra credit).
bool useKDTree;
};

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

    // Brush
    int m_brushType;
    RGBA m_brushColor;
    int m_currentRadius;
    std::unique_ptr<Brush> m_brush;

    // Filter
    float m_p;
    int m_blurRadius;
    float m_scaleX;
    float m_scaleY;

    // Ray
    struct raySetting m_raySetting;
};

#endif // CANVAS2D_H
