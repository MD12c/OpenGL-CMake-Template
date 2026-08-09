#include "main.h"

int main()
{
// Window creation
#pragma region
    Window VIEWPORT;
    Renderer renderer;
    My_ImGui::Init(VIEWPORT.getWindow());
    ShaderManager::LoadAllShaders();

    Scene scene(VIEWPORT.getWindow());
#pragma endregion

    while (!glfwWindowShouldClose(VIEWPORT.getWindow()))
    {
        // VIEWPORT.updateFPS(); // Can be overlayed in imgui
        VIEWPORT.glClearCurrentColor();
        My_ImGui::ShowDockSpace();

        renderer.Render(scene);
        
        My_ImGui::RenderDockSpace();
        glfwSwapBuffers(VIEWPORT.getWindow());
        glfwPollEvents();
    }
#pragma endregion

    return 0;
}
