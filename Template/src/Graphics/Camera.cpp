#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix() {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window) {
    // Orbit keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pitch += speed * 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pitch -= speed * 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) yaw -= speed * 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) yaw += speed * 1.0f;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) radius -= 0.4f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) radius += 0.4f;

    // Clamp pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Compute camera position
    camX = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camY = radius * sin(glm::radians(pitch));
    camZ = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));

    Position = glm::vec3(camX, camY, camZ);

    // Look at origin
	Orientation = glm::normalize(-Position);
}