#include "ImguiSetup.h"

#include <filesystem>
#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace My_ImGui
{
GLFWwindow* m_VIEWPORT = nullptr;
ImGuiIO*    m_io;
bool        m_dockBuild = false;

int My_ImGui::Init(GLFWwindow* VIEWPORT)
{
    if (VIEWPORT == nullptr)
        return 1;

    m_VIEWPORT = VIEWPORT;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |=
        ImGuiConfigFlags_ViewportsEnable |
        ImGuiConfigFlags_DockingEnable |
        ImGuiConfigFlags_NoMouseCursorChange;

    ImFont* font = m_io->Fonts->AddFontFromFileTTF("Assets/Fonts/DejaVuSans.ttf", 26.0f);
    if (!font)
    {
        std::cerr << "Failed to load font, falling back to default font\n";
        m_io->Fonts->AddFontDefault();
    }

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_VIEWPORT, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    m_io->FontGlobalScale = 1.0f;

    return 0;
}

void My_ImGui::ShowDockSpace()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiWindowFlags dockspace_flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpaceHost", nullptr, dockspace_flags);

    ImGui::PopStyleVar(2);

    // Dockspace node
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    if (!m_dockBuild)
    {
        m_dockBuild = true;

        ImGui::DockBuilderRemoveNode(dockspace_id);
        std::cerr << "DockingEnable flag set: " << ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0) << "\n";
        std::cerr << "Nodes count before DockSpace: " << ImGui::GetCurrentContext()->DockContext.Nodes.Data.Size << "\n";
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID dock_main = dockspace_id;

        // Split layout
        ImGuiID dock_top    = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Up, 0.22f, nullptr, &dock_main);
        ImGuiID dock_left   = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Left, 0.25f, nullptr, &dock_main);
        ImGuiID dock_right  = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Right, 0.25f, nullptr, &dock_main);
        ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Down, 0.25f, nullptr, &dock_main);

        // Assign windows to docks
        ImGui::DockBuilderDockWindow("Input", dock_right);
        ImGui::DockBuilderFinish(dockspace_id);
    }
    ImGui::End();
}

void My_ImGui::RenderDockSpace()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup);
    }
}

void My_ImGui::Shutdown()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

void My_ImGui::RenderInterfaceInput()
{
    ImGui::Begin("Input", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Template");
    ImGui::End();
    // ImGui::ShowDemoWindow();
}

void My_ImGui::RenderOverlay(float x, float y, float z,
                             float r, float s, float t)
{
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                             ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

    ImGui::Begin("Debug Overlay", nullptr, flags);
    ImGui::Text("FPS: %1.0f", ImGui::GetIO().Framerate);
    ImGui::Text("Camera Pos: (%.2f, %.2f, %.2f)", x, y, z);
    ImGui::Text("Camera Pos: (%.2f, %.2f, %.2f)", r, s, t);
    ImGui::End();
}

}  // namespace My_ImGui