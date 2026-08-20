#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

namespace My_ImGui
{
extern GLFWwindow* m_VIEWPORT;
extern ImGuiIO*    m_io;
extern bool        m_dockBuilt;

int  Init(GLFWwindow* VIEWPORT);
void ShowDockSpace();
void RenderDockSpace();
void Shutdown();

extern float roughness;
extern float metalic;
void         RenderInterfaceInput();
void         RenderOverlay(float x, float y, float z, float r, float s, float t);
};  // namespace My_ImGui