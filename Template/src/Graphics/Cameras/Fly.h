#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include "Camera.h"

class CameraFly : public Camera
{
private:
    float FOVdeg, nearPlane, farPlane;

	float speed = 0.1f;
	float sensitivity = 100.0f;

public:
	CameraFly(GLFWwindow* window, float FOVdeg, float nearPlane, float farPlane);
    void updateScreenSize() override;
	void Inputs(GLFWwindow* window) override;
    void onScroll(GLFWwindow* win, double xoffset, double yoffset) override;
};

#endif