#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include "Camera.h"

class Orbit : public Camera
{
private:
    glm::vec3 translate  = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up          = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 planeCenter = glm::vec3(2.0f, 0.0f, 2.0f);

    float radius = 5.0f;
    float camX = 0, camY = 0, camZ = 0.0f;
    float yaw = 0, pitch = 0;

    float FOVdeg    = 45.0f;
    float nearPlane = 0.1f, farPlane = 10000.0f;

    float speedYaw   = 1.0f;
    float speedPitch = 1.0f;
    float speedZoom  = 0.1f;

public:
    Orbit(glm::vec3 Position);
    void Inputs(GLFWwindow* window) override;
    void updateScreenSize() override;
    void updateMatrix() override;
    void onScroll(GLFWwindow* win, double xoffset, double yoffset) override {}

    void setSpeeds(float speedYaw, float speedPitch, float speedZoom);
    void setProjection(float FOVdeg, float nearPlane, float farPlane);
    void setPosition(float orbitRadius, float yaw, float pitch);
};

#endif
