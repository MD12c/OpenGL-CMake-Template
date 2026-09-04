#ifndef SETUP
#define SETUP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "glm/glm.hpp"
#include "../Globals.h"

class Window
{
private:
    GLFWwindow* m_window = nullptr;

public:
    static int         width;
    static int         height;
    static std::string windowName;
    static glm::vec3   windowRGB;

    Window(int width, int height, std::string windowName, glm::vec3 windowRGB, glfwPointers* glfwPtr);
    ~Window();

    void StartFrame();
    void EndFrame();
    bool ShouldClose() { return glfwWindowShouldClose(m_window); };

    void updateFPS();

    GLFWwindow* getWindow() { return m_window; }
};

#endif
