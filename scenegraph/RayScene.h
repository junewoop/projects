#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "Canvas2D.h"
#include "Camera.h"
#include <cmath>
#include <vector>

struct ray {
    ray() : p(glm::vec4()), d(glm::vec4()) {}
    ray(glm::vec4 a, glm::vec4 b) : p(a), d(b){}
    glm::vec4 p;
    glm::vec4 d;
};

struct intsct{
    intsct() : t(0.f), i(0){}
    intsct(float a, int b) : t(a), i(b){}
    float t;
    int i;
};

/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */


class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();
    void draw(Canvas2D *canvas, Camera *camera);
protected:
    void setCanvas(Canvas2D *canvas);
    void setCamera(Camera *camera);
    ray createRay(int x, int y);
    float intersectCone(ray one_ray);
    float intersectCube(ray one_ray);
    float intersectCylinder(ray one_ray);
    float intersectSphere(ray one_ray);
    float intersectAt(ray one_ray, int i);
    glm::vec3 normalCone(glm::vec4 p);
    glm::vec3 normalCube(glm::vec4 p);
    glm::vec3 normalCylinder(glm::vec4 p);
    glm::vec3 normalSphere(glm::vec4 p);
    glm::vec3 normalAt(glm::vec4 p, int i);
    void lightingAt(glm::vec4 p, int i, RGBA *data);
    intsct intersect(ray one_ray);

    Canvas2D *m_canvas;
    Camera *m_camera;
    int m_width;
    int m_height;
    RGBA *m_data;
    glm::mat4x4 m_filmToWorld;
    glm::vec4 m_eye;
    std::vector<std::unique_ptr<glm::mat4x4>> m_inverseTransformations;
    std::vector<std::unique_ptr<glm::mat3x3>> m_objectNormalToWorldNormal;
};

#endif // RAYSCENE_H
