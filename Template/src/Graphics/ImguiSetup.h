#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace My_ImGui
{
extern GLFWwindow* m_VIEWPORT;
extern bool        m_dockBuilt;

int  Init(GLFWwindow* VIEWPORT);
void CreateContext();
void ShowDockSpace();
void RenderDockSpace();
void Shutdown();

void RenderInterfaceInput();
};  // namespace My_ImGui