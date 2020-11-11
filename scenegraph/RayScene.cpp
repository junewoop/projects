#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <iostream>

inline unsigned char REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

RayScene::RayScene(Scene &scene) :
    Scene(scene),
    m_canvas(nullptr),
    m_camera(nullptr),
    m_width(0),
    m_height(0),
    m_data(nullptr),
    m_filmToWorld(glm::mat4x4()),
    m_eye(glm::vec4())
{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
    glm::mat4x4 tmp4;
    for (int i = 0; i < m_numPrims; i++){
        std::unique_ptr<glm::mat4x4> tmp_invMat = std::make_unique<glm::mat4x4>();
        *tmp_invMat = glm::inverse(*m_transformations[i]);
        m_inverseTransformations.push_back(std::move(tmp_invMat));
        std::unique_ptr<glm::mat3x3> tmp_objToWor = std::make_unique<glm::mat3x3>();
        tmp4 = *m_transformations[i];
        *tmp_objToWor = glm::inverse(glm::mat3x3(tmp4[0].x, tmp4[1].x, tmp4[2].x,
                tmp4[0].y, tmp4[1].y, tmp4[2].y, tmp4[0].z, tmp4[1].z, tmp4[2].z));
        m_objectNormalToWorldNormal.push_back(std::move(tmp_objToWor));
    }
}

RayScene::~RayScene()
{
}

void RayScene::draw(Canvas2D *canvas, Camera *camera){
    setCanvas(canvas);
    setCamera(camera);
    RGBA *tmp_data = m_data;
    for (int y = 0; y < m_height; y++)
        for (int x = 0; x < m_width; x++)
            intersect(createRay(x, y), tmp_data++);
    m_canvas->update();
}

ray RayScene::createRay(int x, int y){
    glm::vec4 p_world = m_filmToWorld*glm::vec4((2.f*x)/m_width - 1.f, 1.f - (2.f * y)/m_height, -1.f, 1.f);
    glm::vec4 d = glm::vec4(p_world.xyz()-m_eye.xyz(), 0.f);
    d = glm::normalize(d);
    ray res = {m_eye, d};

    return res;
}

void RayScene::intersect(ray one_ray, RGBA* data){
    float t = INFINITY;
    float tmp = 0;
    int i = -1;
    for (int j = 0; j < m_numPrims; j++){
        tmp = intersectAt(one_ray, j);
        if (tmp < t){
            t = tmp;
            i = j;
        }
    }
    if (t != INFINITY)
        lightingAt(one_ray.p + t*one_ray.d, i, data);
    else{
        data->r = 0;
        data->g = 0;
        data->b = 0;
        data->a = 255;
    }
}

void RayScene::lightingAt(glm::vec4 p, int i, RGBA *data){
    glm::vec3 normal = normalAt(p, i);
    glm::vec3 lightVector;
    glm::vec4 color = m_materials[i]->cAmbient;
    glm::vec4 dcolor = m_materials[i]->cDiffuse;
    float dotProduct;
    for (int j = 0; j < m_numLights; j++){
        lightVector = glm::normalize(m_lightData[j]->pos.xyz()-p.xyz());
        dotProduct = std::max(0.f, glm::dot(lightVector, normal));
        color[0] += dotProduct*m_lightData[j]->color[0]*dcolor[0];
        color[1] += dotProduct*m_lightData[j]->color[1]*dcolor[1];
        color[2] += dotProduct*m_lightData[j]->color[2]*dcolor[2];
    }
    data->r = REAL2byte(color[0]);
    data->g = REAL2byte(color[1]);
    data->b = REAL2byte(color[2]);
    data->a = 255;
}

glm::vec3 RayScene::normalAt(glm::vec4 p, int i){
    glm::vec4 object_p = *m_inverseTransformations[i]*p;
    switch(m_types[i]){
    case PrimitiveType::PRIMITIVE_CONE:
        return *m_objectNormalToWorldNormal[i] * normalCone(object_p);
        break;
    case PrimitiveType::PRIMITIVE_CUBE:
        return *m_objectNormalToWorldNormal[i] * normalCube(object_p);
        break;
    case PrimitiveType::PRIMITIVE_CYLINDER:
        return *m_objectNormalToWorldNormal[i] * normalCylinder(object_p);
        break;
    case PrimitiveType::PRIMITIVE_SPHERE:
        return *m_objectNormalToWorldNormal[i] * normalSphere(object_p);
        break;
    default:
        return glm::vec3();
    }
}

glm::vec3 RayScene::normalCone(glm::vec4 p){
    if (fabs(p.y + 0.5) < 0.00001)
        return glm::vec3(0.f, -1.f, 0.f);
    else{
        float x = p.x;
        float z = p.z;
        float d = sqrt(5*x*x+5*z*z);
        return glm::vec3(2.f*x/d, sqrt(0.2f), 2.f*z/d);
    }
}

glm::vec3 RayScene::normalCube(glm::vec4 p){
    if (fabs(p.x + 0.5) < 0.000001)
        return glm::vec3(-1.f, 0.f, 0.f);
    if (fabs(p.x - 0.5) < 0.000001)
        return glm::vec3(1.f, 0.f, 0.f);
    if (fabs(p.y + 0.5) < 0.000001)
        return glm::vec3(0.f, -1.f, 0.f);
    if (fabs(p.y - 0.5) < 0.000001)
        return glm::vec3(0.f, 1.f, 0.f);
    if (fabs(p.z + 0.5) < 0.000001)
        return glm::vec3(0.f, 0.f, -1.f);
    if (fabs(p.z - 0.5) < 0.000001)
        return glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 RayScene::normalCylinder(glm::vec4 p){
    if (fabs(p.y + 0.5) < 0.000001)
        return glm::vec3(0.f, -1.f, 0.f);
    if (fabs(p.y - 0.5) < 0.000001)
        return glm::vec3(0.f, 1.f, 0.f);
    return glm::vec3(2.f*p.x, 0.f, 2.f*p.z);
}

glm::vec3 RayScene::normalSphere(glm::vec4 p){
    return 2.f*p.xyz();
}

float RayScene::intersectAt(ray one_ray, int i){
    ray new_ray;
    new_ray.p = *m_inverseTransformations[i] * one_ray.p;
    new_ray.d = *m_inverseTransformations[i] * one_ray.d;
    switch(m_types[i]){
    case PrimitiveType::PRIMITIVE_CONE:
        return intersectCone(new_ray);
        break;
    case PrimitiveType::PRIMITIVE_CUBE:
        return intersectCube(new_ray);
        break;
    case PrimitiveType::PRIMITIVE_CYLINDER:
        return intersectCylinder(new_ray);
        break;
    case PrimitiveType::PRIMITIVE_SPHERE:
        return intersectSphere(new_ray);
        break;
    default:
        return INFINITY;
        break;
    }
}

float RayScene::intersectCone(ray one_ray){
    float t = INFINITY;
    float t_tmp = t;
    if (fabs(one_ray.d.y) >= 0.000001){
        t_tmp = -(0.5+one_ray.p.y)/one_ray.d.y;
        if (t_tmp > 0 & pow(one_ray.p.x + t_tmp*one_ray.d.x, 2)+ pow(one_ray.p.z + t_tmp*one_ray.d.z, 2) <= 0.25)
            t = t_tmp;
    }
    float a = pow(one_ray.d.x, 2) + pow(one_ray.d.z, 2) - 0.25*pow(one_ray.d.y, 2);
    float b = one_ray.d.x * one_ray.p.x + one_ray.d.z * one_ray.p.z - 0.25 * one_ray.d.y*one_ray.p.y + 0.125 * one_ray.d.y;
    float c = pow(one_ray.p.x, 2) + pow(one_ray.p.z, 2) - 0.25 * pow(one_ray.p.y, 2) + 0.25 * one_ray.p.y - 0.0625;
    if (fabs(a) >= 0.000001 & b*b-a*c > 0){
        t_tmp = (-b-sqrt(b*b-a*c))/a;
        if (t_tmp > 0 & fabs(one_ray.p.y + t_tmp*one_ray.d.y) <= 0.5)
            t = std::min(t, t_tmp);
        t_tmp = (-b+sqrt(b*b-a*c))/a;
        if (t_tmp > 0 & fabs(one_ray.p.y + t_tmp*one_ray.d.y) <= 0.5)
            t = std::min(t, t_tmp);
    }
    return t;
}

float RayScene::intersectCube(ray one_ray){
    float t = INFINITY;
    float t_tmp = t;
    if (one_ray.d.y != 0){
        t_tmp = -(0.5+one_ray.p.y)/one_ray.d.y;
        if (fabs(t_tmp*one_ray.d.x + one_ray.p.x) <= 0.5 & fabs(t_tmp*one_ray.d.z + one_ray.p.z) <= 0.5)
            t = std::min(t_tmp, t);
        t_tmp = (0.5 - one_ray.p.y)/one_ray.d.y;
        if (fabs(t_tmp*one_ray.d.x + one_ray.p.x) <= 0.5 & fabs(t_tmp*one_ray.d.z + one_ray.p.z) <= 0.5)
            t = std::min(t_tmp, t);
    }
    if (one_ray.d.x != 0){
        t_tmp = -(0.5+one_ray.p.x)/one_ray.d.x;
        if (fabs(t_tmp*one_ray.d.z + one_ray.p.z) <= 0.5 & fabs(t_tmp*one_ray.d.y + one_ray.p.y) <= 0.5)
            t = std::min(t_tmp, t);
        t_tmp = (0.5 - one_ray.p.x)/one_ray.d.x;
        if (fabs(t_tmp*one_ray.d.z + one_ray.p.z) <= 0.5 & fabs(t_tmp*one_ray.d.y + one_ray.p.y) <= 0.5)
            t = std::min(t_tmp, t);
;    }
    if (one_ray.d.z != 0){
        t_tmp = -(0.5+one_ray.p.z)/one_ray.d.z;
        if (fabs(t_tmp*one_ray.d.y + one_ray.p.y) <= 0.5 & fabs(t_tmp*one_ray.d.x + one_ray.p.x) <= 0.5)
            t = std::min(t_tmp, t);
        t_tmp = (0.5 - one_ray.p.z)/one_ray.d.z;
        if (fabs(t_tmp*one_ray.d.y + one_ray.p.y) <= 0.5 & fabs(t_tmp*one_ray.d.x + one_ray.p.x) <= 0.5)
            t = std::min(t_tmp, t);
    }
    return t;
}

float RayScene::intersectCylinder(ray one_ray){
    float t = INFINITY;
    float t_tmp = t;
    if (one_ray.d.y != 0){
        t_tmp = -(0.5+one_ray.p.y)/one_ray.d.y;
        if (pow(one_ray.p.x + t_tmp*one_ray.d.x, 2)+ pow(one_ray.p.z + t_tmp*one_ray.d.z, 2) <= 0.25)
            t = std::min(t_tmp, t);
        t_tmp = (0.5-one_ray.p.y)/one_ray.d.y;
        if (pow(one_ray.p.x + t_tmp*one_ray.d.x, 2)+ pow(one_ray.p.z + t_tmp*one_ray.d.z, 2) <= 0.25)
            t = std::min(t_tmp, t);
    }
    float a = one_ray.d.x*one_ray.d.x + one_ray.d.z*one_ray.d.z;
    float b = one_ray.d.x*one_ray.p.x + one_ray.d.z*one_ray.p.z;
    float c = one_ray.p.x*one_ray.p.x + one_ray.p.z*one_ray.p.z - 0.25f;
    if (a > 0 & b*b - a*c > 0){
        t_tmp = (-b-sqrt(b*b-a*c))/a;
        if (t_tmp > 0 & fabs(one_ray.p.y + t_tmp*one_ray.d.y) <= 0.5)
            t = std::min(t, t_tmp);
        t_tmp = (-b+sqrt(b*b-a*c))/a;
        if (t_tmp > 0 & fabs(one_ray.p.y + t_tmp*one_ray.d.y) <= 0.5)
            t = std::min(t, t_tmp);
    }
    return t;
}

float RayScene::intersectSphere(ray one_ray){
    float a = glm::dot(one_ray.d, one_ray.d);
    float b = glm::dot(one_ray.d, one_ray.p);
    float c = glm::dot(one_ray.p, one_ray.p) - 1.25f;
    if (b*b - a*c < 0)
        return INFINITY;
    float t1 =  (-b - sqrt(b*b-a*c))/a;
    float t2 =  (-b + sqrt(b*b-a*c))/a;
    if (t2 < 0)
        return INFINITY;
    else if (t1 < 0)
        return t2;
    else
        return t1;
}

void RayScene::setCanvas(Canvas2D *canvas){
    m_canvas = canvas;
    m_width = canvas->width();
    m_height = canvas->height();
    m_data = canvas->data();
}

void RayScene::setCamera(Camera *camera){
    m_camera = camera;
    m_filmToWorld = glm::inverse(camera->getScaleMatrix() * camera->getViewMatrix());
    m_eye = glm::inverse(camera->getViewMatrix())*glm::vec4(0, 0, 0, 1);
}
