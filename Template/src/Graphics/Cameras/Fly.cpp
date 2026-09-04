#include "Fly.h"

#include "Globals.h"
#include "../Window.h"

CameraFly::CameraFly(float FOVdeg, float nearPlane, float farPlane)
    : FOVdeg(FOVdeg),
      nearPlane(nearPlane),
      farPlane(farPlane)
{
    view = glm::lookAt(Position, Position + Orientation, Up);
    updateScreenSize();
}

void CameraFly::updateScreenSize()
{
    proj = glm::perspective(glm::radians(FOVdeg), (float)Window::width / (float)Window::height, nearPlane, farPlane);
}

void CameraFly::Inputs(GLFWwindow* window)
{
    // Keyboard
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += speed * Orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position += speed * -Orientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Position += speed * Up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        Position += speed * -Up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = 0.4f;
    else
        speed = 0.1f;

    // Mouse
    static bool firstClick = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents rotate spike
        if (firstClick)
        {
            glfwSetCursorPos(window, (Window::width / 2), (Window::height / 2));
            firstClick = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Shifts the cursor coord to be in the middle of the screen and normalizes them
        float rotX = sensitivity * (float)(mouseY - (Window::height / 2)) / Window::height;
        float rotY = sensitivity * (float)(mouseX - (Window::width / 2)) / Window::width;

        // Gets new rotated orientation rotate -> mat4, degree, axis
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // Checks if newOrientation is not too high angle -> vec3, vec3 -> angle between in rad
        if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -Up) <= glm::radians(5.0f))))
            Orientation = newOrientation;

        // Rotates orientation left/right
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Locks the cursor to the middle of the screen
        glfwSetCursorPos(window, (Window::width / 2), (Window::height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
    view = glm::lookAt(Position, Position + Orientation, Up);
}

void CameraFly::onScroll(GLFWwindow* win, double xoffset, double yoffset)
{
    FOVdeg += (float)yoffset * 5;
    FOVdeg = glm::clamp(FOVdeg, 0.0f, 179.0f);
    proj   = glm::perspective(glm::radians(FOVdeg), (float)Window::width / (float)Window::height, nearPlane, farPlane);
}