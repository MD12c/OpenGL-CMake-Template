#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include "Camera.h"

class CameraFly : public Camera
{
private:
    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstClick = true;

    float FOVdeg, nearPlane, farPlane;

	float speed = 0.1f;
	float sensitivity = 100.0f;

public:
	CameraFly(GLFWwindow* window, glm::vec3 Position, float FOVdeg, float nearPlane, float farPlane);
    void updateScreenSize() override;
	void updateMatrix() override;
	void Inputs(GLFWwindow* window) override;
    void onScroll(GLFWwindow* win, double xoffset, double yoffset) override;
};

#endif