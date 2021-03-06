#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"
#include <chrono>
#include <iostream>
#include <tuple>
#include <stack>

RayScene::RayScene(Scene &scene) :
    Scene(scene),
    m_raySetting(),
    m_canvas(nullptr),
    m_width(0),
    m_height(0),
    m_data(nullptr),
    m_camera(nullptr),
    m_filmToWorld(glm::mat4x4()),
    m_eye(glm::vec4()),
    m_root(nullptr),
    m_intersectfunction(nullptr)
{
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
        *tmp_objToWor = glm::transpose(glm::inverse(glm::mat3x3(tmp4)));
        m_objectNormalToWorldNormal.push_back(std::move(tmp_objToWor));
        if (m_materials[i]->textureMap.isUsed){
            std::unique_ptr<QImage> tmp_image = std::make_unique<QImage>(QString::fromStdString(m_materials[i]->textureMap.filename));
            if (tmp_image->isNull())
                m_materials[i]->textureMap.isUsed = false;
            m_textureImages.push_back(std::move(tmp_image));
        }
        else
            m_textureImages.push_back(nullptr);
    }
}

RayScene::~RayScene()
{
    if (m_root) deleteNode(m_root);
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

ray RayScene::createRay(int x, int y){
    glm::vec4 p_world = m_filmToWorld*glm::vec4((2.f*x)/m_width - 1.f, 1.f - (2.f * y)/m_height, -1.f, 1.f);
    glm::vec4 d = glm::vec4(p_world.xyz()-m_eye.xyz(), 0.f);
    d = glm::normalize(d);
    ray res = {m_eye, d};
    return res;
}

intsct RayScene::intersectCone(ray &one_ray){
    float t = INFINITY;
    float u = 0.f, v = 0.f;
    float t_tmp = t;
    if (fabs(one_ray.d.y) >= 0.000015){
        t_tmp = -(0.5+one_ray.p.y)/one_ray.d.y;
        if (t_tmp >= 0 && pow(one_ray.p.x + t_tmp*one_ray.d.x, 2)+ pow(one_ray.p.z + t_tmp*one_ray.d.z, 2) <= 0.250015){
            t = t_tmp;
            v = one_ray.p.z +t_tmp*one_ray.d.z + 0.5f;
            u = one_ray.p.x + t_tmp*one_ray.d.x + 0.5f;
        }
    }
    float a = pow(one_ray.d.x, 2) + pow(one_ray.d.z, 2) - 0.25*pow(one_ray.d.y, 2);
    float b = one_ray.d.x * one_ray.p.x + one_ray.d.z * one_ray.p.z - 0.25 * one_ray.d.y*one_ray.p.y + 0.125 * one_ray.d.y;
    float c = pow(one_ray.p.x, 2) + pow(one_ray.p.z, 2) - 0.25 * pow(one_ray.p.y, 2) + 0.25 * one_ray.p.y - 0.0625;
    if (fabs(a) >= 0.000015 && b*b-a*c > 0){
        t_tmp = (-b-sqrt(b*b-a*c))/a;
        if (t_tmp >= 0 && fabs(one_ray.p.y + t_tmp*one_ray.d.y) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                u = -atan2(one_ray.p.z + t*one_ray.d.z, one_ray.p.x + t*one_ray.d.x)/(2*M_PI);
                v = 0.5f + one_ray.p.y + t*one_ray.d.y;
            }
        }
        t_tmp = (-b+sqrt(b*b-a*c))/a;
        if (t_tmp >= 0 && fabs(one_ray.p.y + t_tmp*one_ray.d.y) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                u = 1.f - atan2(one_ray.p.z + t*one_ray.d.z, one_ray.p.x + t*one_ray.d.x)/(2*M_PI);
                v = one_ray.p.y + t*one_ray.d.y + 0.5f;
            }
        }
    }
    return intsct(t, -1, u-floor(u), v-floor(v));
}

intsct RayScene::intersectCube(ray &one_ray){
    float t = INFINITY;
    float t_tmp = t;
    float u = 0.f, v = 0.f;
    if (one_ray.d.y != 0){
        t_tmp = -(0.5+one_ray.p.y)/one_ray.d.y;
        if (t_tmp >= 0 && fabs(t_tmp*one_ray.d.x + one_ray.p.x) <= 0.500015 && fabs(t_tmp*one_ray.d.z + one_ray.p.z) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                v = one_ray.p.z + t_tmp*one_ray.d.z + 0.5f;
                u = one_ray.p.x + t_tmp*one_ray.d.x + 0.5f;
            }
        }
        t_tmp = (0.5 - one_ray.p.y)/one_ray.d.y;
        if (t_tmp >= 0 && fabs(t_tmp*one_ray.d.x + one_ray.p.x) <= 0.500015 && fabs(t_tmp*one_ray.d.z + one_ray.p.z) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                v = 0.5f - one_ray.p.z - t_tmp*one_ray.d.z;
                u = one_ray.p.x + t_tmp*one_ray.d.x + 0.5f;
            }
        }
    }
    if (one_ray.d.x != 0){
        t_tmp = -(0.5+one_ray.p.x)/one_ray.d.x;
        if (t_tmp >= 0 && fabs(t_tmp*one_ray.d.z + one_ray.p.z) <= 0.500015 && fabs(t_tmp*one_ray.d.y + one_ray.p.y) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                v = 0.5f + one_ray.p.y + t_tmp*one_ray.d.y;
                u = one_ray.p.z + t_tmp*one_ray.d.z + 0.5f;
            }
        }
        t_tmp = (0.5 - one_ray.p.x)/one_ray.d.x;
        if (t_tmp >= 0 && fabs(t_tmp*one_ray.d.z + one_ray.p.z) <= 0.500015 && fabs(t_tmp*one_ray.d.y + one_ray.p.y) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                v = 0.5f + one_ray.p.y + t_tmp*one_ray.d.y;
                u = -one_ray.p.z - t_tmp*one_ray.d.z + 0.5f;
            }
        }
    }
    if (one_ray.d.z != 0){
        t_tmp = -(0.5+one_ray.p.z)/one_ray.d.z;
        if (t_tmp >= 0 && fabs(t_tmp*one_ray.d.y + one_ray.p.y) <= 0.500015 && fabs(t_tmp*one_ray.d.x + one_ray.p.x) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                u = 0.5f - one_ray.p.x - t_tmp*one_ray.d.x;
                v = one_ray.p.y + t_tmp*one_ray.d.y + 0.5f;
            }
        }
        t_tmp = (0.5 - one_ray.p.z)/one_ray.d.z;
        if (t_tmp >= 0 && fabs(t_tmp*one_ray.d.y + one_ray.p.y) <= 0.500015 && fabs(t_tmp*one_ray.d.x + one_ray.p.x) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                u = 0.5f + one_ray.p.x + t_tmp*one_ray.d.x;
                v = one_ray.p.y + t_tmp*one_ray.d.y + 0.5f;
            }
        }
    }
    return intsct(t, -1, u- floor(u), v-floor(v));
}

intsct RayScene::intersectCylinder(ray &one_ray){
    float t = INFINITY;
    float t_tmp = t;
    float u = 0.f, v = 0.f;
    if (one_ray.d.y != 0){
        t_tmp = -(0.5+one_ray.p.y)/one_ray.d.y;
        if (t_tmp >= 0 && pow(one_ray.p.x + t_tmp*one_ray.d.x, 2)+ pow(one_ray.p.z + t_tmp*one_ray.d.z, 2) <= 0.250015){
            if (t_tmp < t){
                t = t_tmp;
                v = 0.5 + one_ray.p.z + t_tmp*one_ray.d.z;
                u = one_ray.p.x + t_tmp*one_ray.d.x + 0.5;
            }
        }
        t_tmp = (0.5-one_ray.p.y)/one_ray.d.y;
        if (t_tmp >= 0 && pow(one_ray.p.x + t_tmp*one_ray.d.x, 2)+ pow(one_ray.p.z + t_tmp*one_ray.d.z, 2) <= 0.250015){
            if (t_tmp < t){
                t = t_tmp;
                v = 0.5 - one_ray.p.z - t_tmp*one_ray.d.z;
                u = one_ray.p.x + t_tmp*one_ray.d.x + 0.5;
            }
        }
    }
    float a = one_ray.d.x*one_ray.d.x + one_ray.d.z*one_ray.d.z;
    float b = one_ray.d.x*one_ray.p.x + one_ray.d.z*one_ray.p.z;
    float c = one_ray.p.x*one_ray.p.x + one_ray.p.z*one_ray.p.z - 0.25f;
    if (a > 0 && b*b - a*c > 0){
        t_tmp = (-b-sqrt(b*b-a*c))/a;
        if (t_tmp >= 0 && fabs(one_ray.p.y + t_tmp*one_ray.d.y) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                u = 1.f - atan2(one_ray.p.z + t*one_ray.d.z, one_ray.p.x + t*one_ray.d.x)/(2*M_PI);
                v = 0.5f + one_ray.p.y + t*one_ray.d.y;
            }
        }
        t_tmp = (-b+sqrt(b*b-a*c))/a;
        if (t_tmp >= 0 && fabs(one_ray.p.y + t_tmp*one_ray.d.y) <= 0.500015){
            if (t_tmp < t){
                t = t_tmp;
                u = 1.f - atan2(one_ray.p.z + t*one_ray.d.z, one_ray.p.x + t*one_ray.d.x)/(2*M_PI);
                v = 0.5f + one_ray.p.y + t*one_ray.d.y;
            }
        }
    }
    return intsct(t, -1, u-floor(u), v-floor(v));
}

intsct RayScene::intersectSphere(ray &one_ray){
    float a = glm::dot(one_ray.d, one_ray.d);
    float b = glm::dot(one_ray.d, one_ray.p);
    float c = glm::dot(one_ray.p, one_ray.p) - 1.25f;
    float u = 0.f, v = 0.f;
    if (b*b - a*c < 0)
        return intsct();
    float t1 =  (-b - sqrt(b*b-a*c))/a;
    float t2 =  (-b + sqrt(b*b-a*c))/a;
    if (t2 < 0)
        return intsct();
    else if (t1 < 0) {
        u = -atan2(one_ray.p.z + t2*one_ray.d.z, one_ray.p.x + t2*one_ray.d.x)/(2*M_PI);
        v = one_ray.p.y + t2*one_ray.d.y;
        v =  0.5f + asin(2.f * v)/M_PI;
        return intsct(t2, -1, u-floor(u), v-floor(v));
    }
    else {
        u = -atan2(one_ray.p.z + t1*one_ray.d.z, one_ray.p.x + t1*one_ray.d.x)/(2*M_PI);
        v = one_ray.p.y + t1*one_ray.d.y;
        v = 0.5f + asin(2.f * v)/M_PI;
        return intsct(t1, -1, u-floor(u), v-floor(v));
    }
}

intsct RayScene::intersectAt(ray &one_ray, int i){
    ray new_ray;
    intsct tmp_intsct = intsct();
    new_ray.p = *m_inverseTransformations[i] * one_ray.p;
    new_ray.d = *m_inverseTransformations[i] * one_ray.d;
    switch(m_types[i]){
    case PrimitiveType::PRIMITIVE_CONE:
        tmp_intsct = intersectCone(new_ray);
        tmp_intsct.i = i;
        break;
    case PrimitiveType::PRIMITIVE_CUBE:
        tmp_intsct = intersectCube(new_ray);
        tmp_intsct.i = i;
        break;
    case PrimitiveType::PRIMITIVE_CYLINDER:
        tmp_intsct = intersectCylinder(new_ray);
        tmp_intsct.i = i;
        break;
    case PrimitiveType::PRIMITIVE_SPHERE:
        tmp_intsct = intersectSphere(new_ray);
        tmp_intsct.i = i;
        break;
    default:
        break;
    }
    return tmp_intsct;
}

glm::vec3 RayScene::normalCone(glm::vec4 p){
    if (fabs(p.y + 0.5) < 0.000015)
        return glm::vec3(0.f, -1.f, 0.f);
    else{
        float x = p.x;
        float z = p.z;
        float d = sqrt(5*x*x+5*z*z);
        return glm::vec3(2.f*x/d, sqrt(0.2f), 2.f*z/d);
    }
}

glm::vec3 RayScene::normalCube(glm::vec4 p){
    if (fabs(p.x + 0.5) < 0.000015)
        return glm::vec3(-1.f, 0.f, 0.f);
    if (fabs(p.x - 0.5) < 0.000015)
        return glm::vec3(1.f, 0.f, 0.f);
    if (fabs(p.y + 0.5) < 0.000015)
        return glm::vec3(0.f, -1.f, 0.f);
    if (fabs(p.y - 0.5) < 0.000015)
        return glm::vec3(0.f, 1.f, 0.f);
    if (fabs(p.z + 0.5) < 0.000015)
        return glm::vec3(0.f, 0.f, -1.f);
    if (fabs(p.z - 0.5) < 0.000015)
        return glm::vec3(0.f, 0.f, 1.f);
    return glm::vec3(0.f);
}

glm::vec3 RayScene::normalCylinder(glm::vec4 p){
    if (fabs(p.y + 0.5) < 0.000015)
        return glm::vec3(0.f, -1.f, 0.f);
    if (fabs(p.y - 0.5) < 0.000015)
        return glm::vec3(0.f, 1.f, 0.f);
    return glm::vec3(2.f*p.x, 0.f, 2.f*p.z);
}

glm::vec3 RayScene::normalSphere(glm::vec4 p){
    return 2.f*p.xyz();
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

glm::vec3 RayScene::lightingAt(glm::vec4 p, int i, glm::vec3 normal, float u, float v){
    glm::vec3 lightVector;
    ray tmp_ray;
    glm::vec4 color = m_materials[i]->cAmbient;
    glm::vec4 dcolor = m_materials[i]->cDiffuse;
    glm::vec4 scolor = m_materials[i]->cSpecular;
    int s = 0, t = 0;
    QImage texImage = QImage();
    float add_r = 0.f, add_g = 0.f, add_b = 0.f;
    float d = 0.f;
    float f_att = 0.f;
    float dotProduct = 0.f;
    QRgb texPixel = QRgb();
    glm::vec4 d_toLight = glm::vec4();
    for (int j = 0; j < m_numLights; j++){
        d = glm::length(m_lightData[j]->pos.xyz()- p.xyz());
        // positional lighting
        if ((m_lightData[j]->type == LightType::LIGHT_POINT) && m_raySetting.usePointLights){
            lightVector = glm::normalize(m_lightData[j]->pos.xyz()-p.xyz());
            d_toLight = glm::vec4(lightVector, 0.f);
            tmp_ray = ray(p + 0.001f*d_toLight, d_toLight);
            if ((this->*m_intersectfunction)(tmp_ray).t > d-0.001f || !m_raySetting.useShadows){
                dotProduct = std::max(0.f, glm::dot(lightVector, normal));
                add_r = dotProduct*dcolor[0];
                add_g = dotProduct*dcolor[1];
                add_b = dotProduct*dcolor[2];
                if (m_materials[i]->textureMap.isUsed && m_raySetting.useTextureMapping){
                    texImage = *m_textureImages[i];
                    s = static_cast<int>(u*m_materials[i]->textureMap.repeatU*texImage.width()) % texImage.width();
                    t = static_cast<int>((1-v)*m_materials[i]->textureMap.repeatV*texImage.height()) % texImage.height();
                    texPixel = m_textureImages[i]->pixel(s, t);
                    add_r = (1.f-m_materials[i]->blend) * add_r + m_materials[i]->blend * (qRed(texPixel) / 255.f) * dotProduct;
                    add_g = (1.f-m_materials[i]->blend) * add_g + m_materials[i]->blend * (qGreen(texPixel) / 255.f) * dotProduct;
                    add_b = (1.f-m_materials[i]->blend) * add_b + m_materials[i]->blend * (qBlue(texPixel) / 255.f) * dotProduct;
                }
                dotProduct = glm::max(0.f, glm::dot(2*glm::dot(normal, lightVector)*normal - lightVector,
                                      glm::normalize(m_eye.xyz() - p.xyz())));
                add_r += pow(dotProduct, m_materials[i]->shininess)*scolor[0];
                add_g += pow(dotProduct, m_materials[i]->shininess)*scolor[1];
                add_b += pow(dotProduct, m_materials[i]->shininess)*scolor[2];
                f_att = std::min(1.f/(glm::dot(m_lightData[j]->function, glm::vec3(1, d, d*d))), 1.f);
                color[0] += f_att*m_lightData[j]->color[0]*add_r;
                color[1] += f_att*m_lightData[j]->color[1]*add_g;
                color[2] += f_att*m_lightData[j]->color[2]*add_b;
            }
        }
        // directional lighting
        if ((m_lightData[j]->type == LightType::LIGHT_DIRECTIONAL) && m_raySetting.useDirectionalLights){
            lightVector = glm::normalize(-m_lightData[j]->dir.xyz());
            d_toLight = glm::vec4(lightVector, 0.f);
            tmp_ray = ray(p + 0.1f*d_toLight, d_toLight);
            if ((this->*m_intersectfunction)(tmp_ray).t > d-0.001f || !m_raySetting.useShadows){
                dotProduct = std::max(0.f, glm::dot(lightVector, normal));
                add_r = dotProduct*dcolor[0];
                add_g = dotProduct*dcolor[1];
                add_b = dotProduct*dcolor[2];
                if (m_materials[i]->textureMap.isUsed && m_raySetting.useTextureMapping){
                    texImage = *m_textureImages[i];
                    s = static_cast<int>(u*m_materials[i]->textureMap.repeatU*texImage.width()) % texImage.width();
                    t = static_cast<int>((1-v)*m_materials[i]->textureMap.repeatV*texImage.height()) % texImage.height();
                    texPixel = m_textureImages[i]->pixel(s, t);
                    add_r = (1.f-m_materials[i]->blend) * add_r + m_materials[i]->blend * (qRed(texPixel) / 255.f) * dotProduct;
                    add_g = (1.f-m_materials[i]->blend) * add_g + m_materials[i]->blend * (qGreen(texPixel) / 255.f) * dotProduct;
                    add_b = (1.f-m_materials[i]->blend) * add_b + m_materials[i]->blend * (qBlue(texPixel) / 255.f) * dotProduct;
                }
                dotProduct = glm::max(0.f, glm::dot(2*glm::dot(normal, lightVector)*normal - lightVector,
                                      glm::normalize(m_eye.xyz() - p.xyz())));
                add_r += pow(dotProduct, m_materials[i]->shininess)*scolor[0];
                add_g += pow(dotProduct, m_materials[i]->shininess)*scolor[1];
                add_b += pow(dotProduct, m_materials[i]->shininess)*scolor[2];
                color[0] += m_lightData[j]->color[0]*add_r;
                color[1] += m_lightData[j]->color[1]*add_g;
                color[2] += m_lightData[j]->color[2]*add_b;
            }
        }
    }
    return glm::vec3(color[0], color[1], color[2]);
}

intsct RayScene::intersect(ray &one_ray){
    intsct tmp = intsct();
    intsct best = intsct();
    for (int i = 0; i < m_numPrims; i++){
        tmp = intersectAt(one_ray, i);
        if (tmp.t < best.t){
            best = tmp;
        }
    }
    return best;
}

glm::vec3 RayScene::recursiveLight(ray &cur_ray, intsct cur_intsct, int num_left){
    if (cur_intsct.t == INFINITY)
        return glm::vec3(0.f);
    glm::vec3 normal = glm::normalize(normalAt(cur_ray.p + cur_intsct.t*cur_ray.d, cur_intsct.i));
    glm::vec3 nextColor = lightingAt(cur_ray.p + cur_intsct.t*cur_ray.d, cur_intsct.i, normal, cur_intsct.u, cur_intsct.v);
    if (num_left == 0)
        return nextColor;
    else{
        glm::vec3 inVector = cur_ray.d.xyz();
        glm::vec4 nextd = glm::vec4(inVector - 2.f*glm::dot(inVector, normal)*normal, 0.f);
        ray next_ray = ray(cur_ray.p + cur_intsct.t*cur_ray.d + 0.001f*nextd, nextd);
        intsct next_intsct = (this->*m_intersectfunction)(next_ray);
        glm::vec3 addColor = recursiveLight(next_ray, next_intsct, num_left - 1);
        nextColor[0] += addColor[0] * m_materials[cur_intsct.i]->cReflective.r;
        nextColor[1] += addColor[1] * m_materials[cur_intsct.i]->cReflective.g;
        nextColor[2] += addColor[2] * m_materials[cur_intsct.i]->cReflective.b;
        return nextColor;
    }
}

void RayScene::constructKDTree(){
    auto start = std::chrono::steady_clock::now();
    m_root = new KDNode();
    AABB tmp_box = AABB();
    for (int i = 0; i < m_numPrims; i++){
        tmp_box = AABB(*m_transformations[i]);
        tmp_box.prim_index = i;
        m_root->x_min = std::min(m_root->x_min, tmp_box.x_min);
        m_root->y_min = std::min(m_root->y_min, tmp_box.y_min);
        m_root->z_min = std::min(m_root->z_min, tmp_box.z_min);
        m_root->x_max = std::max(m_root->x_max, tmp_box.x_max);
        m_root->y_max = std::max(m_root->y_max, tmp_box.y_max);
        m_root->z_max = std::max(m_root->z_max, tmp_box.z_max);
        m_boxes.push_back(tmp_box);
    }
    m_root->numBox = m_numPrims;
    m_root->boxes = new AABB*[m_numPrims];
    for (int i = 0; i < m_numPrims; i++) m_root->boxes[i] = &m_boxes[i];
    m_root->recurSplitKDTree();
    // construction time
    auto end = std::chrono::steady_clock::now();
    std::cout << "KDTree construction time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " milliseconds" << std::endl;
}

void RayScene::deleteNode(KDNode *node){
    if (!node->is_leaf){
        deleteNode(node->child_1);
        deleteNode(node->child_2);
    }
    delete[] node->boxes;
    delete node;
}

intsct RayScene::intersectKDTree(ray &one_ray){
    for (int i = 0; i < m_numPrims; i++) m_boxes[i].visited = false;
    std::pair<float, float> t_global = m_root->intersectBox(one_ray);
    if (t_global.second == -INFINITY) return intsct();

    std::stack<std::tuple<KDNode*, float, float>> s;
    s.push(std::make_tuple(m_root, t_global.first, t_global.second));
    std::tuple<KDNode*, float, float> tup = std::make_tuple(nullptr, 0.f, 0.f);
    KDNode *cur_node = nullptr, *near_node = nullptr, *far_node = nullptr;
    float t_min = 0.f, t_max = 0.f, t = 0.f;
    intsct cur_intsct = intsct(), tmp_intsct = intsct();
    bool b = true;
    while(!s.empty()){
        tup = s.top();
        s.pop();
        cur_node = std::get<0>(tup);
        t_min = std::get<1>(tup);
        t_max = std::get<2>(tup);
        // Traverse until a leaf node is reached
        while(!cur_node->is_leaf){
            switch (cur_node->axis) {
            case X:
                t = (cur_node->boundary - one_ray.p.x)/one_ray.d.x;
                b = (one_ray.p.x < cur_node->boundary);
                break;
            case Y:
                t = (cur_node->boundary - one_ray.p.y)/one_ray.d.y;
                b = (one_ray.p.y < cur_node->boundary);
                break;
            case Z:
                t = (cur_node->boundary - one_ray.p.z)/one_ray.d.z;
                b = (one_ray.p.z < cur_node->boundary);
                break;
            }
            near_node = b ? cur_node->child_1 : cur_node->child_2;
            far_node = b ? cur_node->child_2 : cur_node->child_1;
            if (t >= t_max || t < 0) cur_node = near_node;
            else if (t <= t_min) cur_node = far_node;
            else{
                s.push(std::make_tuple(far_node, t, t_max));
                cur_node = near_node;
                t_max = t;
            }
        }
        // Loop through primitives inside the found leaf node
        for(int i = 0; i < cur_node->numBox; i++){
            if (cur_node->boxes[i]->visited)
                continue;
            tmp_intsct = intersectAt(one_ray, cur_node->boxes[i]->prim_index);
            if (tmp_intsct.t != INFINITY) cur_node->boxes[i]->visited = true;
            if (tmp_intsct.t < cur_intsct.t) cur_intsct = tmp_intsct;
        }
        if (cur_intsct.t >= t_min -0.00015f && cur_intsct.t <= t_max + 0.00015f)
            return cur_intsct;
    }
    return intsct();
}

void RayScene::renderPixel(int x, int y){
    ray one_ray = createRay(x, y);
    intsct one_intsct = (this->*m_intersectfunction)(one_ray);
    glm::vec3 color = recursiveLight(one_ray, one_intsct, m_raySetting.reflectionDepth);
    *(m_data + (y *m_width + x)) = RGBA(REAL2byte(color.x), REAL2byte(color.y), REAL2byte(color.z), 255);
}

void RayScene::draw(Canvas2D *canvas, Camera *camera, raySetting ray_setting){
    // initialization
    setCanvas(canvas);
    setCamera(camera);
    m_raySetting = ray_setting;
    if (m_raySetting.useKDTree){
        constructKDTree();
        m_intersectfunction = &RayScene::intersectKDTree;
    }
    else m_intersectfunction = &RayScene::intersect;
    if (!m_raySetting.useReflection)
        m_raySetting.reflectionDepth = 0;
    // else m_raySetting.reflectionDepth = <ENTER VALUE HERE>;

    // Start rendering
    auto start = std::chrono::steady_clock::now();
    for (int y = 0; y < m_height; y++)
        for (int x = 0; x < m_width; x++)
            renderPixel(x, y);
    m_canvas->update();

    // Print out rendering time
    auto end = std::chrono::steady_clock::now();
    std::cout << "Rendering time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " milliseconds" << std::endl;
}
