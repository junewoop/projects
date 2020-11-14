#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "Canvas2D.h"
#include "Camera.h"
#include <cmath>
#include <vector>

struct ray {
    glm::vec4 p;
    glm::vec4 d;
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
    void intersect(ray one_ray, RGBA *data);

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
