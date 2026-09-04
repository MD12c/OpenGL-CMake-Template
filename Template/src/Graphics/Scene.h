#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include <vector>
#include <functional>

#include "Lighting/LightResources.h"
#include "Models/Transform.h"
#include "Models\Skybox.h"
#include "Cameras/Camera.h"

class Camera;

class Scene
{
public:
    const float zNear = 0.1f;
    const float zFar  = 400.0f;

    GLFWwindow*               glfwWindowPtr;
    std::function<void(void)> imguiFunctions = {};

    Transform          worldTransform;
    Skybox             skybox;
    LightResources     lightResources;
    std::vector<Model> models;

    int                                  activeCam = 0;
    std::vector<std::unique_ptr<Camera>> cameras;

    Scene(GLFWwindow* glfwWindowPtr);
    ~Scene();

    const Camera& getActiveCamera() const { return *cameras.at(activeCam); }
    void          Resize(int w, int h);
};

#endif