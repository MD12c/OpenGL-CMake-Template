#include "Camera.h"
#include "Globals.h"

std::vector<Camera*> Camera::cameras = {};

Camera::Camera()
{
    cameras.push_back(this);
}

void Camera::Activate(GLFWwindow* window, GLuint uniformLoc)
{
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    glfwSetScrollCallback(window, ScrollCallback);
}

glm::vec2 Camera::screenToWorld(const glm::vec2& pos)
{
    float ndcX = (2.0f * (float)pos.x) / width - 1.0f;
    float ndcY = 1.0f - (2.0f * (float)pos.y) / height;

    glm::mat4 invCamera = glm::inverse(cameraMatrix);
    glm::vec4 world     = invCamera * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

    return glm::vec2(world.x, world.y);
}

void Camera::ScrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{
    auto* ptr = static_cast<glfwPointers*>(glfwGetWindowUserPointer(win));

    if (ptr && ptr->camera)
        ptr->camera->onScroll(win, xoffset, yoffset);
}