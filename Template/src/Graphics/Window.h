#ifndef SETUP
#define SETUP

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Window {
private:
    GLFWwindow* m_window = nullptr;

public:
    Window();
    void glClearCurrentColor();
    void updateFPS();

    GLFWwindow* getWindow();
    ~Window();
};

#endif
