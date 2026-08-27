#include "App.h"

App::App()
    : window(), renderer(), scene(window.getWindow())
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
    glfwPtr.camera   = scene.cameras[scene.activeCam].get();
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
            scene.lights.at(0).setPosition(camera.Position);
            scene.lights.at(0).setDirection(camera.Orientation);
        }

        static bool firstClick_C = true;
        if (glfwGetKey(window.getWindow(), GLFW_KEY_C) == GLFW_PRESS && firstClick_C)
        {
            scene.activeCam = !scene.activeCam;
            firstClick_C = false;
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