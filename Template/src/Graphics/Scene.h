#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include <vector>
#include <functional>

#include "Lighting\LightSystem.h"
#include "Models/Model.h"
#include "Models/Transform.h"
#include "Models\Skybox.h"
#include "Cameras/Camera.h"

class Camera;

class Scene
{
public:
    const float zNear = 0.1f;
    const float zFar  = 400.0f;

    LightSystem               lightSystem;
    GLFWwindow*               glfwWindowPtr;
    std::function<void(void)> imguiFunctions = {};

    Transform                       worldTransform;
    Skybox                          skybox;
    std::vector<Model>              models;
    std::vector<LightSystem::Light> lights;

    int                                  activeCam = 0;
    std::vector<std::unique_ptr<Camera>> cameras;

    Scene(GLFWwindow* glfwWindowPtr);
    ~Scene();

    const Camera& getActiveCamera() const;
};

#endif