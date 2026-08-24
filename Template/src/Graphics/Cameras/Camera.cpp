#include "Camera.h"
#include "Globals.h"

std::vector<Camera*> Camera::cameras = {};

Camera::Camera(GLFWwindow* window)
{
    glfwPtr.camera = this;
    cameras.push_back(this);
    glfwSetScrollCallback(window, ScrollCallback);
}

void Camera::updateUniforms(ShaderIDs shaderID) const
{
    ShaderManager::Activate(shaderID);
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(ShaderManager::getLoc(shaderID, "camPos"), position.x, position.y, position.z);
}

glm::vec2 Camera::screenToWorld(const glm::vec2& pos)
{
    float ndcX = (2.0f * (float)pos.x) / width - 1.0f;
    float ndcY = 1.0f - (2.0f * (float)pos.y) / height;

    glm::mat4 invCamera = glm::inverse(proj * view);
    glm::vec4 world     = invCamera * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

    if (world.w != 0.0f)
        world /= world.w;

    return glm::vec2(world.x, world.y);
}

void Camera::ScrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{
    auto* ptr = static_cast<glfwPointers*>(glfwGetWindowUserPointer(win));

    if (ptr && ptr->camera)
        ptr->camera->onScroll(win, xoffset, yoffset);
}

glm::mat4 Camera::getRotationMat()
{
    glm::mat4 noTransView = glm::mat4(glm::mat3(view));
    return proj * noTransView;
}