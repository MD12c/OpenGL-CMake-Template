#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include <vector>
#include <functional>

#include "LightSystem.h"
#include "Models/Model.h"
#include "Skybox.h"
#include "Cameras/Camera.h"


class Scene
{
public:
    LightSystem lightSystem;
    GLFWwindow* glfwWindowPtr;
    std::function<void(void)> imguiFunctions = {};

    Skybox              skybox;
    std::vector<Model>  models;

    int activeCam = 0;
    std::vector<std::unique_ptr<Camera>> cameras;

    Scene(GLFWwindow* glfwWindowPtr);
    ~Scene();
};

#endif