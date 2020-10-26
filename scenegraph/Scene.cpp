#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"
#include <iostream>

Scene::Scene() :
    m_numLights(0),
    m_lightIndex(0),
    m_shapeIndex(0),
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

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()
    CS123SceneGlobalData tmp_global;
    parser->getGlobalData(tmp_global);
    sceneToFill->setGlobal(tmp_global);

    CS123SceneLightData tmp_light;
    sceneToFill->m_numLights = parser->getNumLights();
    sceneToFill->m_lightData.resize(0);
    sceneToFill->m_lightIndex = 0;
    for (int i = 0; i < sceneToFill->m_numLights; i++){
        if (parser->getLightData(i, tmp_light)){
            sceneToFill->addLight(tmp_light);
            sceneToFill->m_lightIndex++;
        }
    }

    CS123SceneNode *curNode = parser->getRootNode();
    glm::mat4x4 curMat = glm::mat4(1.f);
    sceneToFill->m_shapeIndex = 0;
    sceneToFill->m_primitive.resize(0);
    sceneToFill->m_transformation.resize(0);
    sceneToFill->addPrimitiveDFS(curNode, curMat);
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    m_primitive.push_back(&scenePrimitive);
    m_transformation.push_back(&matrix);
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
            tmp_mat = tmp_mat * glm::rotate(glm::degrees(curTrans->angle),
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
    m_lightData.push_back(&sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
    // std::cout << m_global.ka<< m_global.kd<< m_global.ks<< m_global.kt << std::endl;
}
