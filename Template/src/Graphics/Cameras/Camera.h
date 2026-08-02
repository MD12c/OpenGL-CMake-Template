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

#include "../Shader.h"

class Camera
{
public:
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    glm::vec3 position     = glm::vec3(0.0f, 0.0f, 0.0f);

    static std::vector<Camera*> cameras;

    Camera(GLFWwindow* window);
    static void ScrollCallback(GLFWwindow* win, double xoffset, double yoffset);
    void        updateUniforms(GLuint cameraMatrixLoc, GLuint positionLoc);
    glm::vec2   screenToWorld(const glm::vec2& pos);

    virtual void updateScreenSize()                                        = 0;
    virtual void updateMatrix()                                            = 0;
    virtual void Inputs(GLFWwindow* window)                                = 0;
    virtual void onScroll(GLFWwindow* win, double xoffset, double yoffset) = 0;
};
#endif