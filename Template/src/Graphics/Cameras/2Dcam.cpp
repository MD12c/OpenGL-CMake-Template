#include "2Dcam.h"
#include "Globals.h"

Camera2D::Camera2D(GLFWwindow* window)
{
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    updateOrtho();
}

void Camera2D::onScroll(GLFWwindow* win, double xoffset, double yoffset)
{
    auto* ptr = static_cast<glfwPointers*>(glfwGetWindowUserPointer(win));

    if (auto* cam = dynamic_cast<Camera2D*>(ptr->camera))
    {
        double posX;
        double posY;
        glfwGetCursorPos(ptr->window->getWindow(), &posX, &posY);
        glm::vec2 worldMouseBefore = cam->screenToWorld(glm::vec2(posX, posY));

        float sensitivity = 0.2f;
        cam->zoom         = glm::clamp(expf(logf(cam->zoom) + -(float)yoffset * sensitivity), 0.125f, 64.0f);
        cam->scale        = glm::scale(glm::mat4(1.0f), glm::vec3(cam->zoom, cam->zoom, 1.0f));
        cam->updateMatrix();

        glm::vec2 worldMouseAfter = cam->screenToWorld(glm::vec2(posX, posY));
        glm::vec2 d               = worldMouseBefore - worldMouseAfter;

        cam->translation = glm::translate(cam->translation, glm::vec3(d, 0.0f));
        cam->updateMatrix();
    }
};

void Camera2D::updateOrtho()
{
    float aspect = (float)width / (float)height;
    proj         = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    updateMatrix();
}

void Camera2D::updateMatrix()
{
    cameraMatrix = proj * glm::inverse(translation * scale);
}

void Camera2D::Inputs(GLFWwindow* window)
{
    static double posXprev = 0;
    static double posYprev = 0;

    double posX;
    double posY;
    glfwGetCursorPos(window, &posX, &posY);

    glm::vec2 d = glm::vec2(posX - posXprev, posY - posYprev);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glm::vec2 worldCurr  = screenToWorld(glm::vec2(posX, posY));
        glm::vec2 worldPrev  = screenToWorld(glm::vec2(posXprev, posYprev));
        glm::vec2 worldDelta = worldPrev - worldCurr;

        translation = glm::translate(translation, glm::vec3(worldDelta, 0.0f));
        updateMatrix();
    }

    posXprev = posX;
    posYprev = posY;
}
