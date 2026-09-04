#include "App.h"

#include "Graphics/ImguiSetup.h"

App::App()
    : window(2560, 1440, "Template", glm::vec3(0.7f), &glfwPtr), renderer(2.0f), scene(window.getWindow())
{
    My_ImGui::Init(window.getWindow());
    setResizePointers();
}

App::~App()
{
}

void App::setResizePointers()
{
    glfwPtr.window   = &window;
    glfwPtr.scene    = &scene;
    glfwPtr.renderer = &renderer;
}

void App::Update()
{
    // VIEWPORT.updateFPS(); // Can be overlayed in imgui
    timeCrnt = glfwGetTime();
    timeDiff = timeCrnt - timePrev;
    if (timeDiff >= 1.0 / 60.0)
    {
        timePrev = timeCrnt;

        if (!My_ImGui::m_io->WantCaptureMouse)
            scene.cameras[scene.activeCam]->Inputs(scene.glfwWindowPtr);

        Camera& camera = *scene.cameras[scene.activeCam];
        if (glfwGetKey(window.getWindow(), GLFW_KEY_B) == GLFW_PRESS)
        {
            scene.lightResources.pointLights.at(0).setPosition(camera.Position);
            // scene.lightResources.directionLights.at(0).setPosition(camera.Position);
            // scene.lightResources.directionLights.at(0).setDirection(camera.Orientation);
            // scene.lightResources.spotLights.at(0).setPosition(camera.Position);
            // scene.lightResources.spotLights.at(0).setDirection(camera.Orientation);
        }

        static bool firstClick_C = true;
        if (glfwGetKey(window.getWindow(), GLFW_KEY_C) == GLFW_PRESS && firstClick_C)
        {
            scene.activeCam < static_cast<int>(scene.cameras.size() - 1) ? scene.activeCam++ : scene.activeCam = 0;
            firstClick_C    = false;
        }
        else if (glfwGetKey(window.getWindow(), GLFW_KEY_C) == GLFW_RELEASE)
            firstClick_C = true;

        // for (auto& model : scene.models)
        //     model.setMeshMetalicRoughness(-1, My_ImGui::metalic, My_ImGui::roughness);
    }
}

void App::RunFrame()
{
    Update();
    setResizePointers();
    window.StartFrame();
    My_ImGui::ShowDockSpace();

    renderer.Render(scene);

    My_ImGui::RenderDockSpace();
    window.EndFrame();

#ifdef BENCHMARK
    GPUInstrumentor::Get().ResolvePending();
#endif
}