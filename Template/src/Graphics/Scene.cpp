#include "Scene.h"

#include "ImguiSetup.h"
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
    : lightResources(zNear, zFar), skybox(HDRimage), glfwWindowPtr(glfwWindowPtr), worldTransform({}, {}, glm::vec3(0.02f))
{
    // cameras.emplace_back(std::make_unique<Camera2D>(glfwWindowPtr));
    // cameras.emplace_back(std::make_unique<CameraOrbit>(glfwWindowPtr));
    cameras.emplace_back(std::make_unique<CameraFly>(glfwWindowPtr, 45.0f, zNear, zFar));
    cameras.emplace_back(std::make_unique<CameraFly>(glfwWindowPtr, 45.0f, zNear, zFar));

    // models.emplace_back("Assets/Models/crow/scene.gltf");
    //   models.emplace_back("Assets/Models/crow.obj");

    // models.emplace_back("Assets/Models/Brick wall/wall.gltf");
    // models.emplace_back("Assets/Models/Brick wall/wall.obj");

    // models.emplace_back("Assets/Models/Lava Wall/lava.obj");

    // models.emplace_back("Assets/Models/ignore/Sphere/Metal/Sphere.gltf");
    // models.emplace_back("Assets/Models/ignore/sponza_palace/scene.gltf");
    // models.emplace_back("Assets/Models/Sphere/Sphere.obj");

    // models.emplace_back("Assets/Models/icoSphere.obj");

    glm::vec3 lightPosition    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 lightOrientation = glm::vec3(-0.15f, 1.0f, -1.0f);

    // lightResources.addDirectionLight(lightPosition, lightOrientation, glm::vec3(10.0f, 10.0f, 10.0f), -35.0f, 35.0f, -35.0f, 35.0f);
    // lightResources.addSpotLight(lightPosition, lightOrientation, glm::vec3(10.0f, 10.0f, 10.0f), 90.0f, 0.95f, 0.90f);
    lightResources.addPointLight(lightPosition, glm::vec3(10.0f, 10.0f, 10.0f));
    // lightResources.addPointLight(lightPosition, glm::vec3(100.0f, 100.0f, 100.0f));
    // lightResources.addPointLight(lightPosition, glm::vec3(100.0f, 100.0f, 100.0f));
    // lightResources.addPointLight(lightPosition, glm::vec3(100.0f, 100.0f, 100.0f));
    // lightResources.addPointLight(lightPosition, glm::vec3(100.0f, 100.0f, 100.0f));
    // lightResources.addPointLight(lightPosition, glm::vec3(100.0f, 100.0f, 100.0f));
    // lightResources.addPointLight(lightPosition, glm::vec3(100.0f, 100.0f, 100.0f));
    // lightResources.addPointLight(lightPosition, glm::vec3(100.0f, 100.0f, 100.0f));

    imguiFunctions = [&]()
    {
        const Camera& cam = *cameras[activeCam];
        My_ImGui::RenderOverlay(
            cam.Position.x, cam.Position.y, cam.Position.z,
            cam.Orientation.x, cam.Orientation.y, cam.Orientation.z);
        //My_ImGui::RenderInterfaceInput();
    };
}

Scene::~Scene()
{
}

const Camera& Scene::getActiveCamera() const
{
    return *cameras.at(activeCam);
}