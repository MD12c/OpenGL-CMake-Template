#include "ImguiSetup.h"

namespace My_ImGui
{
GLFWwindow* m_VIEWPORT = nullptr;
bool m_dockBuild = false;

int My_ImGui::Init(GLFWwindow* VIEWPORT)
{
    if (VIEWPORT == nullptr)
        return 1;

    m_VIEWPORT = VIEWPORT;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |=
        ImGuiConfigFlags_ViewportsEnable |
        ImGuiConfigFlags_DockingEnable;

    ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF("Assets/Fonts/DejaVuSans.ttf", 26.0f);
    if (!font)
        std::cerr << "Failed to load font\n";

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_VIEWPORT, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::GetIO().FontGlobalScale = 1.0f;

    return 0;
}

void My_ImGui::ShowDockSpace()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiWindowFlags dockspace_flags =
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration;

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
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID dock_main = dockspace_id;
        ImGuiID dock_top;
        ImGuiID dock_left;
        ImGuiID dock_right;
        ImGuiID dock_bottom;

        // Split layout
        dock_top    = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Up, 0.22f, nullptr, &dock_main);
        dock_left   = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Left, 0.25f, nullptr, &dock_main);
        dock_right  = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Right, 0.25f, nullptr, &dock_main);
        dock_bottom = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Down, 0.25f, nullptr, &dock_main);

        // Assign windows to docks
        ImGui::DockBuilderDockWindow("Input", dock_top);
        ImGui::DockBuilderFinish(dockspace_id);
    }
    ImGui::End();
}

void My_ImGui::RenderDockSpace()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
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
    ImGui::ShowDemoWindow();
    ImGui::End();
}
}  // namespace My_ImGui