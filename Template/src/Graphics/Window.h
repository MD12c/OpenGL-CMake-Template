#ifndef SETUP
#define SETUP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
private:
    GLFWwindow* m_window = nullptr;

public:
    Window();
    void StartFrame();
    void EndFrame();
    bool ShouldClose() { return glfwWindowShouldClose(m_window); };

    void updateFPS();

    GLFWwindow* getWindow();
    ~Window();
};

#endif
