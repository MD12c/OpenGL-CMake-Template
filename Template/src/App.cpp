#include "App.h"

App::App()
    : window(), renderer(), scene(window.getWindow())
{
    My_ImGui::Init(window.getWindow());
    setResizePointers();
    quad = new Square();
}

App::~App()
{
    delete quad;
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
        if (glfwGetKey(window.getWindow(), GLFW_KEY_B) == GLFW_PRESS)
        {
            scene.lights.at(0).setPosition(scene.cameras[scene.activeCam]->position);
            scene.lights.at(0).setDirection(scene.cameras[scene.activeCam]->Orientation);
        }
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
}