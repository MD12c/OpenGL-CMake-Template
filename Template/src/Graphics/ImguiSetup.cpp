#include"ImguiSetup.h"

Imgui::Imgui(GLFWwindow* VIEWPORT) : m_VIEWPORT(VIEWPORT) {}

void Imgui::CreateContext() {
    // Created ImGUI context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    m_io = &ImGui::GetIO(); (void)m_io;
	m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	m_io->Fonts->AddFontFromFileTTF("Assets/Fonts/DejaVuSans.ttf", 16.0f);
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_VIEWPORT, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().FontGlobalScale = 1.0f;
}

void Imgui::ShowDockSpace() {
    // ImGui initializeation
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiWindowFlags dockspace_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
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
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f,0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    static bool dock_built = false;
    if (!dock_built)
    {
        dock_built = true;

        ImGui::DockBuilderRemoveNode(dockspace_id); // clear
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID dock_main = dockspace_id;
        ImGuiID dock_left;
        ImGuiID dock_right;
        ImGuiID dock_bottom;

        // Split layout
        dock_left   = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Left,   0.25f, nullptr, &dock_main);
        dock_right  = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Right,  0.25f, nullptr, &dock_main);
        dock_bottom = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Down,   0.25f, nullptr, &dock_main);

        // Assign windows to docks
        ImGui::DockBuilderDockWindow("Function input", dock_right);
        ImGui::DockBuilderFinish(dockspace_id);
    }
    ImGui::End();
}

void Imgui::RenderDockSpace() {
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

ImGuiIO* Imgui::getGuiContext() const {
    return m_io;
}