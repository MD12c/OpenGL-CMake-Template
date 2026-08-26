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
    : lightSystem(zNear, zFar), skybox(HDRimage), glfwWindowPtr(glfwWindowPtr)
{
    // cameras.emplace_back(std::make_unique<Camera2D>(glfwWindowPtr));
    // cameras.emplace_back(std::make_unique<CameraOrbit>(glfwWindowPtr));
    cameras.emplace_back(std::make_unique<CameraFly>(glfwWindowPtr, 45.0f, zNear, zFar));

    // models.emplace_back("Assets/Models/crow/scene.gltf");
    //   models.emplace_back("Assets/Models/crow.obj");

    // models.emplace_back("Assets/Models/Brick wall/wall.gltf");
    // models.emplace_back("Assets/Models/Brick wall/wall.obj");

    // models.emplace_back("Assets/Models/Lava Wall/lava.obj");

    // models.emplace_back("Assets/Models/ignore/Sphere/Metal/Sphere.gltf");
    models.emplace_back("Assets/Models/ignore/sponza_palace/scene.gltf");
    // models.emplace_back("Assets/Models/Sphere/Sphere.obj");

    // models.emplace_back("Assets/Models/icoSphere.obj");

    glm::vec3 lightPosition    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 lightOrientation = glm::vec3(-0.15f, 1.0f, -1.0f);

    // lightSystem.addDirectionLight(lights, lightPosition, lightOrientation, glm::vec3(10.0f, 10.0f, 10.0f), -35.0f, 35.0f, -35.0f, 35.0f);
    lightSystem.addSpotLight(lights, lightPosition, lightOrientation, glm::vec3(10.0f, 10.0f, 10.0f), 90.0f, 0.95f, 0.90f);
    // lightSystem.addPointLight(lights, lightPosition, glm::vec3(100.0f, 100.0f, 100.0f));

    imguiFunctions = [&]()
    {
        My_ImGui::RenderOverlay(cameras[activeCam]->Position.x, cameras[activeCam]->Position.y, cameras[activeCam]->Position.z,
                                cameras[activeCam]->Orientation.x, cameras[activeCam]->Orientation.y, cameras[activeCam]->Orientation.z);
        My_ImGui::RenderInterfaceInput();
    };
}

Scene::~Scene()
{
}

const Camera& Scene::getActiveCamera() const
{
    return *cameras.at(activeCam);
}