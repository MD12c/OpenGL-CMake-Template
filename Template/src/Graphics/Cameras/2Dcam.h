#ifndef CAMERA2D_CAMERA_H
#define CAMERA2D_CAMERA_H

#include "Camera.h"

class Camera2D : public Camera
{
private:
    glm::mat4 proj        = glm::mat4(1.0f);
    glm::mat4 translation = glm::mat4(1.0f);
    glm::mat4 scale       = glm::mat4(1.0f);

    float zoom = 1.0f;

public:
    Camera2D(GLFWwindow* window);
    
    void updateOrtho();
    void updateMatrix() override;
    void Inputs(GLFWwindow* window) override;
    void onScroll(GLFWwindow* win, double xoffset, double yoffset) override;
};

#endif