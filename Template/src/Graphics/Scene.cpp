#include "Scene.h"

#include "ImguiSetup.h"
#include "Cameras/Orbit.h"
#include "Cameras/2Dcam.h"
#include "Cameras/Fly.h"

static std::string facesCubemap[6] = {
    "Assets/Textures/Skybox/right.jpg",
    "Assets/Textures/Skybox/left.jpg",
    "Assets/Textures/Skybox/top.jpg",
    "Assets/Textures/Skybox/bottom.jpg",
    "Assets/Textures/Skybox/front.jpg",
    "Assets/Textures/Skybox/back.jpg"
};

static std::string HDRimage = "Assets/Textures/cloudbox/clouds.hdr";

Scene::Scene(GLFWwindow* glfwWindowPtr)
    : lightSystem(0.1f, 400.0f), skybox(HDRimage), glfwWindowPtr(glfwWindowPtr)
{
    // cameras.emplace_back(std::make_unique<Camera2D>(glfwWindowPtr));
    // cameras.emplace_back(std::make_unique<CameraOrbit>(glfwWindowPtr));
    cameras.emplace_back(std::make_unique<CameraFly>(glfwWindowPtr, 45.0f, 0.1f, 10000.0f));

    models.emplace_back("Assets/Models/crow/scene.gltf");
    //   models.emplace_back("Assets/Models/crow.obj");

    // models.emplace_back("Assets/Models/Brick wall/wall.gltf");
    // models.emplace_back("Assets/Models/Brick wall/wall.obj");

    // models.emplace_back("Assets/Models/Lava Wall/lava.obj");

    models.emplace_back("Assets/Models/Sphere/Metal/Sphere.gltf");
    // models.emplace_back("Assets/Models/Sphere/Sphere.obj");

    // models.emplace_back("Assets/Models/icoSphere.obj");

    // glm::vec3 lightPosition = glm::vec3(0.0f, 30.0f, 0.0f);
    // glm::vec3 lightPosition = glm::vec3(-4.5f, 17.0f, 3.0f);
    // glm::vec3 lightPosition    = glm::vec3(-3.0f, 11.5f, 11.5f);
    glm::vec3 lightPosition    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 lightOrientation = glm::vec3(-0.15f, 1.0f, -1.0f);

    //lightSystem.addDirectionLight(lights, lightPosition, lightOrientation, glm::vec3(1.0f, 1.0f, 1.0f), -35.0f, 35.0f, -35.0f, 35.0f);
    // lightSystem.addSpotLight(lights, lightPosition, lightOrientation, glm::vec3(1.0f, 1.0f, 1.0f), 90.0f, 0.1f, 0.95f);
    lightSystem.addPointLight(lights, lightPosition, glm::vec3(3.0f, 3.0f, 3.0f));

    imguiFunctions = [&]()
    {
        My_ImGui::RenderOverlay(cameras[activeCam]->position.x, cameras[activeCam]->position.y, cameras[activeCam]->position.z,
                                cameras[activeCam]->Orientation.x, cameras[activeCam]->Orientation.y, cameras[activeCam]->Orientation.z);
        //My_ImGui::RenderInterfaceInput();
    };
}

Scene::~Scene()
{
}