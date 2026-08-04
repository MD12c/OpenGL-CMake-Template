#include "Orbit.h"
#include "Globals.h"

Orbit::Orbit(GLFWwindow* window)
    : Camera(window)
{
    position    = calculatePos() + focusPoint;
    Orientation = glm::normalize(-position + focusPoint);
    view        = glm::lookAt(position, position + Orientation, Up);
    proj        = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
    updateScreenSize();
}

void Orbit::setProjection(float FOVdeg, float nearPlane, float farPlane)
{
    this->FOVdeg    = FOVdeg;
    this->nearPlane = nearPlane;
    this->farPlane  = farPlane;
}
void Orbit::setPosition(float orbitRadius, float yaw, float pitch)
{
    this->radius = orbitRadius;
    this->yaw    = yaw;
    this->pitch  = pitch;
}
void Orbit::setSpeeds(float speedYaw, float speedPitch, float speedZoom)
{
    this->speedYaw   = speedYaw;
    this->speedPitch = speedPitch;
    this->speedZoom  = speedZoom;
}

glm::vec3 Orbit::calculatePos()
{
    float camX = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    float camY = radius * sin(glm::radians(pitch));
    float camZ = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    return glm::vec3(camX, camY, camZ);
}

void Orbit::Inputs(GLFWwindow* window)
{
    static bool firstClick = true;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Rotate
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents rotate spike
        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Shifts the cursor coord to be in the middle of the screen and normalizes them
        float rotX = speedPitch * (float)(mouseX - (width / 2)) / width;
        float rotY = speedYaw * (float)(mouseY - (height / 2)) / height;

        yaw -= rotX;
        pitch += rotY;

        // Clamp pitch to avoid flipping
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        position = calculatePos() + focusPoint;

        // Look at origin
        Orientation = glm::normalize(-position + focusPoint);

        // Locks the cursor to the middle of the screen
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        // Move
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float moveX = speedMove * (float)(mouseX - (width / 2)) / width;
        float moveY = speedMove * (float)(mouseY - (height / 2)) / height;

        glm::vec3 newRight = glm::normalize(glm::cross(Orientation, Up));
        glm::vec3 newUp    = glm::normalize(glm::cross(newRight, Orientation));
        focusPoint += -newRight * moveX + newUp * moveY;
        position = calculatePos() + focusPoint;

        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }

    view = glm::lookAt(position, position + Orientation, Up);
}

void Orbit::onScroll(GLFWwindow* win, double xoffset, double yoffset)
{
    radius      = glm::clamp(expf(logf(radius) + -(float)yoffset * speedZoom), 0.1f, 1000.0f);
    position    = calculatePos() + focusPoint;
    Orientation = glm::normalize(-position + focusPoint);
    view        = glm::lookAt(position, position + Orientation, Up);
}

void Orbit::updateScreenSize()
{
    float aspect = (float)width / (float)height;
    proj         = glm::perspective(glm::radians(FOVdeg), aspect, nearPlane, farPlane);
}