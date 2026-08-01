#include "Orbit.h"
#include "Globals.h"

Orbit::Orbit(glm::vec3 Position)
{
    translate   = Position;
    view       = glm::lookAt(translate, translate + Orientation, Up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
    updateMatrix();
}

void Orbit::setProjection(float FOVdeg, float nearPlane, float farPlane, float speed)
{
    this->FOVdeg    = FOVdeg;
    this->nearPlane = nearPlane;
    this->farPlane  = farPlane;
    this->speed     = speed;
}
void Orbit::setParameters(float orbitRadius, float yaw, float pitch)
{
    this->radius = orbitRadius;
    this->yaw    = yaw;
    this->pitch  = pitch;
}

void Orbit::Inputs(GLFWwindow* window)
{
    // Orbit keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pitch += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pitch -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) yaw -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) yaw += speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) radius -= 0.4f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) radius += 0.4f;

    // Clamp pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Compute camera position
    camX = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camY = radius * sin(glm::radians(pitch));
    camZ = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));

    translate = glm::vec3(camX, camY, camZ);

    // Look at origin
    Orientation = glm::normalize(-translate);

    updateMatrix();
}

void Orbit::updateMatrix()
{
    view         = glm::lookAt(translate, translate + Orientation, Up);
    cameraMatrix = projection * view;
}

void Orbit::updateScreenSize()
{
    float aspect = (float)width / (float)height;
    projection   = glm::perspective(glm::radians(FOVdeg), aspect, nearPlane, farPlane);
    updateMatrix();
}