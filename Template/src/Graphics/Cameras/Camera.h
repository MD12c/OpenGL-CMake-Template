#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../Shaders/ShaderManager.h"

class Camera
{
public:
    glm::mat4 proj  = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);

    glm::vec3 position    = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up          = glm::vec3(0.0f, 1.0f, 0.0f);

    static std::vector<Camera*> cameras;

    Camera(GLFWwindow* window);
    static void ScrollCallback(GLFWwindow* win, double xoffset, double yoffset);
    void        updateUniforms(ShaderID shaderID) const;
    glm::vec2   screenToWorld(const glm::vec2& pos);

    virtual void updateScreenSize()                                        = 0;
    virtual void Inputs(GLFWwindow* window)                                = 0;
    virtual void onScroll(GLFWwindow* win, double xoffset, double yoffset) = 0;

    glm::mat4 getRotationMat();
};
#endif