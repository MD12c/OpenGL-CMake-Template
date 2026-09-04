#include "Camera.h"

#include "Globals.h"
#include "../Shaders/Shader.h"
#include "../Window.h"

Camera::Camera()
{
}

void Camera::updateUniforms(ShaderID shaderID) const
{
    Shader::Activate(shaderID);
    glUniformMatrix4fv(Shader::getLoc(shaderID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(Shader::getLoc(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(Shader::getLoc(shaderID, "camPos"), Position.x, Position.y, Position.z);
}

glm::vec2 Camera::screenToWorld(const glm::vec2& pos)
{
    float ndcX = (2.0f * (float)pos.x) / Window::width - 1.0f;
    float ndcY = 1.0f - (2.0f * (float)pos.y) / Window::height;

    glm::mat4 invCamera = glm::inverse(proj * view);
    glm::vec4 world     = invCamera * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

    if (world.w != 0.0f)
        world /= world.w;

    return glm::vec2(world.x, world.y);
}

glm::mat4 Camera::getRotationMat()
{
    glm::mat4 noTransView = glm::mat4(glm::mat3(view));
    return proj * noTransView;
}