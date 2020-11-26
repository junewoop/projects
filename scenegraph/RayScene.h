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
    intsct() : t(INFINITY), i(-1), u(0.f), v(0.f){}
    intsct(float a, int b, float c, float d) : t(a), i(b), u(c), v(d){}
    float t;
    int i;
    float u;
    float v;
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
    void draw(Canvas2D *canvas, Camera *camera, raySetting ray_setting);
protected:
    void setCanvas(Canvas2D *canvas);
    void setCamera(Camera *camera);
    ray createRay(int x, int y);
    intsct intersectCone(ray one_ray);
    intsct intersectCube(ray one_ray);
    intsct intersectCylinder(ray one_ray);
    intsct intersectSphere(ray one_ray);
    intsct intersectAt(ray one_ray, int i);
    glm::vec3 normalCone(glm::vec4 p);
    glm::vec3 normalCube(glm::vec4 p);
    glm::vec3 normalCylinder(glm::vec4 p);
    glm::vec3 normalSphere(glm::vec4 p);
    glm::vec3 normalAt(glm::vec4 p, int i);
    glm::vec3 lightingAt(glm::vec4 p, int i, glm::vec3 normal, float u = 0.5f, float v = 0.5f);
    intsct intersect(ray one_ray);
    glm::vec3 recursiveLight(ray cur_ray, intsct cur_intsct, int num_left);

    Canvas2D *m_canvas;
    Camera *m_camera;
    int m_width;
    int m_height;
    RGBA *m_data;
    glm::mat4x4 m_filmToWorld;
    glm::vec4 m_eye;
    std::vector<std::unique_ptr<glm::mat4x4>> m_inverseTransformations;
    std::vector<std::unique_ptr<glm::mat3x3>> m_objectNormalToWorldNormal;
    std::vector<std::unique_ptr<QImage>> m_textureImages;
    raySetting m_raySetting;
};

#endif // RAYSCENE_H
