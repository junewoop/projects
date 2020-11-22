#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"
#include "glm/gtx/transform.hpp"

Scene::Scene() :
    m_numLights(0),
    m_numPrims(0),
    m_global({0.f, 0.f, 0.f, 0.f})
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.
    m_numLights = scene.m_numLights;
    m_numPrims = scene.m_numPrims;
    for (int i = 0; i < m_numLights; i++){
        std::unique_ptr<CS123SceneLightData> tmp_light = std::make_unique<CS123SceneLightData>();
        *tmp_light = *scene.m_lightData[i];
        m_lightData.push_back(std::move(tmp_light));
    }
    for (int i = 0; i < m_numPrims; i++){
        std::unique_ptr<CS123SceneMaterial> tmp_material = std::make_unique<CS123SceneMaterial>();
        std::unique_ptr<glm::mat4x4> tmp_trans = std::make_unique<glm::mat4x4>();
        *tmp_material = *scene.m_materials[i];
        *tmp_trans = *scene.m_transformations[i];
        m_materials.push_back(std::move(tmp_material));
        m_types.push_back(scene.m_types[i]);
        m_transformations.push_back(std::move(tmp_trans));
    }
}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // store global data from parser
    CS123SceneGlobalData tmp_global;
    parser->getGlobalData(tmp_global);
    sceneToFill->setGlobal(tmp_global);

    // store light data from parser
    CS123SceneLightData tmp_light;
    sceneToFill->m_numLights = parser->getNumLights();
    sceneToFill->m_lightData.reserve(sceneToFill->m_numLights);
    for (int i = 0; i < sceneToFill->m_numLights; i++)
        if (parser->getLightData(i, tmp_light))
            sceneToFill->addLight(tmp_light);

    // store object data from parser
    CS123SceneNode *curNode = parser->getRootNode();
    sceneToFill->m_numPrims = 0;
    glm::mat4x4 curMat = glm::mat4(1.f);
    sceneToFill->addPrimitiveDFS(curNode, curMat);
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    // add material data
    std::unique_ptr<CS123SceneMaterial> tmp_material = std::make_unique<CS123SceneMaterial>();
    *tmp_material = scenePrimitive.material;
    tmp_material->cAmbient.r *= m_global.ka;
    tmp_material->cAmbient.g *= m_global.ka;
    tmp_material->cAmbient.b *= m_global.ka;
    tmp_material->cDiffuse.r *= m_global.kd;
    tmp_material->cDiffuse.g *= m_global.kd;
    tmp_material->cDiffuse.b *= m_global.kd;
    tmp_material->cSpecular.r *= m_global.ks;
    tmp_material->cSpecular.g *= m_global.ks;
    tmp_material->cSpecular.b *= m_global.ks;
    m_materials.push_back(std::move(tmp_material));

    // add type data
    m_types.push_back(scenePrimitive.type);

    // add transformation data
    std::unique_ptr<glm::mat4x4> tmp_mat = std::make_unique<glm::mat4x4>();
    *tmp_mat = matrix;
    m_transformations.push_back(std::move(tmp_mat));

    m_numPrims++;
}

void Scene::addPrimitiveDFS(CS123SceneNode *curNode, const glm::mat4x4 &curMat){
    // compute transformation matrix
    int numTrans = curNode->transformations.size();
    glm::mat4x4 tmp_mat = curMat;
    CS123SceneTransformation* curTrans;
    for (int i = 0; i < numTrans; i++){
        curTrans = curNode->transformations[i];
        switch (curTrans->type) {
        case TRANSFORMATION_TRANSLATE:
            tmp_mat = tmp_mat * glm::translate(curTrans->translate);
            break;
        case TRANSFORMATION_ROTATE:
            tmp_mat = tmp_mat * glm::rotate(curTrans->angle,
                                            curTrans->rotate);
            break;
        case TRANSFORMATION_SCALE:
            tmp_mat = tmp_mat * glm::scale(curTrans->scale);
            break;
        case TRANSFORMATION_MATRIX:
            tmp_mat = tmp_mat * curTrans->matrix;
        }
    }

    // add all primitives
    int numPrims = curNode->primitives.size();
    for (int i = 0; i < numPrims; i ++)
        addPrimitive(*curNode->primitives[i], tmp_mat);

    // recursive call on children
    int numChild = curNode->children.size();
    for (int i = 0; i < numChild; i++)
        addPrimitiveDFS(curNode->children[i], tmp_mat);
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    std::unique_ptr<CS123SceneLightData> tmp_light = std::make_unique<CS123SceneLightData>();
    *tmp_light = sceneLight;
    m_lightData.push_back(std::move(tmp_light));
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
}
