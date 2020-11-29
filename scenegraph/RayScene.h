#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Canvas2D.h"
#include "Camera.h"
#include "RayBox.h"

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
    // setting
    void setCanvas(Canvas2D *canvas);
    void setCamera(Camera *camera);

    // ray and intersect
    ray createRay(int x, int y);
    intsct intersectCone(ray &one_ray);
    intsct intersectCube(ray &one_ray);
    intsct intersectCylinder(ray &one_ray);
    intsct intersectSphere(ray &one_ray);
    intsct intersectAt(ray &one_ray, int i);
    intsct intersect(ray &one_ray);

    // computing normals
    glm::vec3 normalCone(glm::vec4 p);
    glm::vec3 normalCube(glm::vec4 p);
    glm::vec3 normalCylinder(glm::vec4 p);
    glm::vec3 normalSphere(glm::vec4 p);
    glm::vec3 normalAt(glm::vec4 p, int i);

    // computing lightings
    glm::vec3 lightingAt(glm::vec4 p, int i, glm::vec3 normal, float u = 0.5f, float v = 0.5f);
    glm::vec3 recursiveLight(ray &cur_ray, intsct cur_intsct, int num_left);
    void renderPixel(int x, int y);

    // kd trees
    void constructKDTree();
    void deleteNode(KDNode *node);
    intsct intersectKDTree(ray &one_ray);

    //setting
    raySetting m_raySetting;

    // Canvas data
    Canvas2D *m_canvas;
    int m_width;
    int m_height;
    RGBA *m_data;

    // Camera data
    Camera *m_camera;
    glm::mat4x4 m_filmToWorld;
    glm::vec4 m_eye;

    // transfrmations and texture mappings
    std::vector<std::unique_ptr<glm::mat4x4>> m_inverseTransformations;
    std::vector<std::unique_ptr<glm::mat3x3>> m_objectNormalToWorldNormal;
    std::vector<std::unique_ptr<QImage>> m_textureImages;

    // kd tree
    KDNode *m_root;
    std::vector<AABB> m_boxes;
    intsct (RayScene::*m_intersectfunction)(ray&);
};

#endif // RAYSCENE_H
