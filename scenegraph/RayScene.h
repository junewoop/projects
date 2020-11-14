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

struct box {
    box(): x_min(INFINITY), x_max(-INFINITY), y_min(INFINITY), y_max(-INFINITY), z_min(INFINITY), z_max(-INFINITY) {}
    box(float a, float b, float c, float d, float e, float f): x_min(a), x_max(b), y_min(c), y_max(d), z_min(e), z_max(f) {}
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;
};

enum DivideAxis {X, Y, Z};

struct KDNode : box{
    KDNode(): box(), is_leaf(false), axis(X), child_1(nullptr), child_2(nullptr) {}
    KDNode(float a, float b, float c, float d, float e, float f): box(a, b, c, d, e, f), is_leaf(false), axis(X), child_1(nullptr), child_2(nullptr) {}
    bool is_leaf;
    DivideAxis axis;
    KDNode *child_1;
    KDNode *child_2;
    std::vector<int> primitives;
};

std::vector<glm::vec4> CHARPOINTS{glm::vec4(-0.5f, 0.f, 0.f, 1.f),
                           glm::vec4(0.5f, 0.f, 0.f, 1.f),
                           glm::vec4(0.f, -0.5f, 0.f, 1.f),
                           glm::vec4(0.f, 0.5f, 0.f, 1.f),
                           glm::vec4(0.f, 0.f, -0.5f, 1.f),
                           glm::vec4(0.f, 0.f, 0.5f, 1.f)};

box computeBox(glm::mat4x4 &inverse){
    box b = box();
    glm::vec4 p = glm::vec4();
    for (int i = 0; i < 6; i ++){
        p = inverse * CHARPOINTS[i];
        if(p.x < b.x_min) b.x_min = p.x;
        if(p.x > b.x_max) b.x_max = p.x;
        if(p.y < b.x_min) b.y_min = p.y;
        if(p.y > b.x_max) b.y_max = p.y;
        if(p.z < b.x_min) b.z_min = p.z;
        if(p.z > b.x_max) b.z_max = p.z;
    }
    return b;
}

/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */


class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();
    void draw(Canvas2D *canvas, Camera *camera, bool useMultiThreading);
    void setKDTree(bool useKDTree);
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
    void constructKDTree();
    void recurKDTree(KDNode *node);
    void deleteNode(KDNode *node);

    Canvas2D *m_canvas;
    Camera *m_camera;
    int m_width;
    int m_height;
    RGBA *m_data;
    glm::mat4x4 m_filmToWorld;
    glm::vec4 m_eye;
    std::vector<std::unique_ptr<glm::mat4x4>> m_inverseTransformations;
    std::vector<std::unique_ptr<glm::mat3x3>> m_objectNormalToWorldNormal;
    std::vector<std::unique_ptr<box>> m_boxes;
    bool m_useKDTree;
    KDNode *m_root;
};

#endif // RAYSCENE_H
