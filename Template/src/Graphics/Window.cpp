#include "Window.h"
#include "../Globals.h"
#include "Cameras/Camera.h"
#include "FrameBuffers/MSAABuffer.h"
#include "FrameBuffers/Framebuffer.h"

Window::Window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
    if (m_window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, &glfwPtr);
    glfwPtr.window = this;

    auto resizeCallback = [](GLFWwindow* win, int w, int h)
    {
        auto* ptr = static_cast<glfwPointers*>(glfwGetWindowUserPointer(win));
        glViewport(0, 0, w, h);
        width  = w;
        height = h;
        ptr->camera->updateScreenSize();
        ptr->framebuffer->Resize(w, h);
        ptr->msaabuffer->Resize(w, h);
    };

    glfwSetFramebufferSizeCallback(m_window, resizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glViewport(0, 0, width, height);
    glClearColor(windowRGB[0], windowRGB[1], windowRGB[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window);

    glfwSwapInterval(0);
}

void Window::updateFPS()
{
    // FPS
    static double       prevTime = 0.0;
    static double       crntTime = 0.0;
    static double       timeDiff;
    static unsigned int counter = 0;

    crntTime = glfwGetTime();
    timeDiff = crntTime - prevTime;
    counter++;

    if (timeDiff >= 1.0 / 30.0)
    {
        std::string FPS      = std::to_string((1.0 / timeDiff) * counter);
        std::string ms       = std::to_string((timeDiff / counter) * 1000.0);
        std::string newTitle = windowName + "   " + FPS + " FPS / " + ms + " ms";
        glfwSetWindowTitle(m_window, newTitle.c_str());
        prevTime = crntTime;
        counter  = 0;
    }
}

GLFWwindow* Window::getWindow()
{
    return m_window;
}

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Window::StartFrame()
{
    glClearColor(windowRGB[0], windowRGB[1], windowRGB[2], 1.0f);
}

void Window::EndFrame()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}
