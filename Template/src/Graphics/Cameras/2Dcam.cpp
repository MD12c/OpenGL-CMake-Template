#include "2Dcam.h"
#include "Globals.h"
#include "../Window.h"

Camera2D::Camera2D()
{
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    updateScreenSize();
}

void Camera2D::onScroll(GLFWwindow* win, double xoffset, double yoffset)
{
    auto* ptr = static_cast<glfwPointers*>(glfwGetWindowUserPointer(win));

    double posX;
    double posY;
    glfwGetCursorPos(ptr->window->getWindow(), &posX, &posY);
    glm::vec2 worldMouseBefore = screenToWorld(glm::vec2(posX, posY));

    zoom  = glm::clamp(expf(logf(zoom) + -(float)yoffset * sensitivity), 0.125f, 64.0f);
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, 1.0f));

    view = glm::inverse(glm::translate(glm::mat4(1.0f), Position) * scale);

    glm::vec2 worldMouseAfter = screenToWorld(glm::vec2(posX, posY));
    glm::vec2 d               = worldMouseBefore - worldMouseAfter;

    Position += glm::vec3(d, 0.0f);
    view = glm::inverse(glm::translate(glm::mat4(1.0f), Position) * scale);
};

void Camera2D::updateScreenSize()
{
    float aspect = (float)Window::width / (float)Window::height;
    proj         = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
}

void Camera2D::Inputs(GLFWwindow* window)
{
    static double posXprev = 0;
    static double posYprev = 0;

    double posX;
    double posY;
    glfwGetCursorPos(window, &posX, &posY);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glm::vec2 worldCurr  = screenToWorld(glm::vec2(posX, posY));
        glm::vec2 worldPrev  = screenToWorld(glm::vec2(posXprev, posYprev));
        glm::vec2 worldDelta = worldPrev - worldCurr;

        Position += glm::vec3(worldDelta, 0.0f);

        view = glm::inverse(glm::translate(glm::mat4(1.0f), Position) * scale);
    }

    posXprev = posX;
    posYprev = posY;
}
