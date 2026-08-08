#pragma once
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
void RenderOverlay(float x, float y, float z,float r, float s, float t);
};  // namespace My_ImGui