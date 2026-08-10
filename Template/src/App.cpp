#include "App.h"

App::App()
    : window(), renderer(), scene(window.getWindow())
{
    My_ImGui::Init(window.getWindow());
    ShaderManager::LoadAllShaders();
}

App::~App()
{
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
    }
}

void App::RunFrame()
{
    Update();
    window.StartFrame();
    My_ImGui::ShowDockSpace();

    renderer.Render(scene);
    
    My_ImGui::RenderDockSpace();
    window.EndFrame();
}