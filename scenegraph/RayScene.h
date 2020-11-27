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

struct AABA{
    AABA(): x_min(INFINITY), x_max(-INFINITY), y_min(INFINITY), y_max(-INFINITY), z_min(INFINITY), z_max(-INFINITY),
    prim_index(-1), visited(false) {}
    AABA(float a, float b, float c, float d, float e, float f, int i): x_min(a), x_max(b), y_min(c), y_max(d), z_min(e), z_max(f),
    prim_index(i), visited(false) {}
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;
    int prim_index;
    bool visited;
};

enum DivideAxis {X, Y, Z};

struct KDNode : AABA{
    KDNode(): AABA(), is_leaf(false), axis(X), child_1(nullptr), child_2(nullptr),
        boxes(nullptr), boundary(0.f), numBox(0), trial(0) {}
    KDNode(float a, float b, float c, float d, float e, float f): AABA(a, b, c, d, e, f, -1),
        is_leaf(false), axis(X), child_1(nullptr), child_2(nullptr),
        boxes(nullptr), boundary(0.f), numBox(0), trial(0) {}
    bool is_leaf;
    DivideAxis axis;
    KDNode *child_1;
    KDNode *child_2;
    AABA **boxes;
    float boundary;
    int numBox;
    int trial;
};


static std::vector<glm::vec4> CHARPOINTS{glm::vec4(-0.5f, 0.f, 0.f, 1.f),
            glm::vec4(0.5f, 0.f, 0.f, 1.f),
            glm::vec4(0.f, -0.5f, 0.f, 1.f),
            glm::vec4(0.f, 0.5f, 0.f, 1.f),
            glm::vec4(0.f, 0.f, -0.5f, 1.f),
            glm::vec4(0.f, 0.f, 0.5f, 1.f)};

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
    intsct intersectCone(ray &one_ray);
    intsct intersectCube(ray &one_ray);
    intsct intersectCylinder(ray &one_ray);
    intsct intersectSphere(ray &one_ray);
    intsct intersectAt(ray &one_ray, int i);
    glm::vec3 normalCone(glm::vec4 p);
    glm::vec3 normalCube(glm::vec4 p);
    glm::vec3 normalCylinder(glm::vec4 p);
    glm::vec3 normalSphere(glm::vec4 p);
    glm::vec3 normalAt(glm::vec4 p, int i);
    glm::vec3 lightingAt(glm::vec4 p, int i, glm::vec3 normal, float u = 0.5f, float v = 0.5f);
    intsct intersect(ray &one_ray);
    glm::vec3 recursiveLight(ray &cur_ray, intsct cur_intsct, int num_left);
    AABA computeAABA(glm::mat4x4 &inverse);
    void constructKDTree();
    void recurKDTree(KDNode *node);
    void deleteNode(KDNode *node);
    intsct intersectKDTree(ray &one_ray);
    intsct intersectBox(ray &one_ray, AABA box);
    intsct traverseKDTree(ray &one_ray, KDNode *cur_node, intsct cur_intsct);

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
    KDNode *m_root;
    std::vector<AABA> m_boxes;
    intsct (RayScene::*m_intersectfunction)(ray&);
};

#endif // RAYSCENE_H
