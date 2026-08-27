#ifndef CAMERA2D_CAMERA_H
#define CAMERA2D_CAMERA_H

#include "Camera.h"

class Camera2D : public Camera
{
private:
    float zoom        = 1.0f;
    float sensitivity = 0.2f;

public:
    Camera2D(GLFWwindow* window);

    void  updateScreenSize() override;
    void  Inputs(GLFWwindow* window) override;
    void  onScroll(GLFWwindow* win, double xoffset, double yoffset) override;
    float getFOV() const override { return -1.0f; }
};

#endif