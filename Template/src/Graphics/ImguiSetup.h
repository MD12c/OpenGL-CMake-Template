#pragma once
#include"imgui.h"
#include"imgui_internal.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Imgui {
public:
    GLFWwindow* m_VIEWPORT;
    ImGuiIO* m_io;

    Imgui(GLFWwindow* VIEWPORT);
    void CreateContext();
    void ShowDockSpace();
    void RenderDockSpace();
    ImGuiIO* getGuiContext() const;
};