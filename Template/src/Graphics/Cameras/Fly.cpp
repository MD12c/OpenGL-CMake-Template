#include "Fly.h"
#include "Globals.h"

CameraFly::CameraFly(GLFWwindow* window, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
    : Camera(window),
      FOVdeg(FOVdeg),
      nearPlane(nearPlane),
      farPlane(farPlane)
{
    this->position = position;
    view = glm::lookAt(position, position + Orientation, Up);
    updateScreenSize();
}

void CameraFly::updateScreenSize()
{
    proj = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
}

void CameraFly::Inputs(GLFWwindow* window)
{
    // Keyboard
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += speed * Orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position += speed * -glm::normalize(glm::cross(Orientation, Up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position += speed * -Orientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += speed * glm::normalize(glm::cross(Orientation, Up));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += speed * Up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        position += speed * -Up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = 0.4f;
    else
        speed = 0.1f;

    // Mouse
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
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
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // Gets new rotated orientation rotate -> mat4, degree, axis
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // Checks if newOrientation is not too high angle -> vec3, vec3 -> angle between in rad
        if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -Up) <= glm::radians(5.0f))))
            Orientation = newOrientation;

        // Rotates orientation left/right
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Locks the cursor to the middle of the screen
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        firstClick = true;
    }
    view = glm::lookAt(position, position + Orientation, Up);
}

void CameraFly::onScroll(GLFWwindow* win, double xoffset, double yoffset)
{
    FOVdeg += (float)yoffset * 5;
    FOVdeg = glm::clamp(FOVdeg, 0.0f, 179.0f);
    proj   = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
}