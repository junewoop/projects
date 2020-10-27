#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"

class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

protected:
    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    // Adds all primitive objects for the scene.
    virtual void addPrimitiveDFS(CS123SceneNode *curNode, const glm::mat4x4 &matrix);

    int m_numLights;
    int m_numPrims;
    CS123SceneGlobalData m_global;
    std::vector<std::unique_ptr<CS123SceneLightData>> m_lightData;
    std::vector<std::unique_ptr<CS123SceneMaterial>> m_materials;
    std::vector<PrimitiveType> m_types;
    std::vector<std::unique_ptr<glm::mat4x4>> m_transformations;
};

#endif // SCENE_H
