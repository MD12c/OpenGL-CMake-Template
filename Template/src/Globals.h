#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Graphics/Window.h"
#include "Graphics/Cameras/Camera.h"

inline int         width        = 2560;
inline int         height       = 1440;
inline std::string windowName   = "Template";
inline GLfloat     windowRGB[3] = {
    0.8f, 0.8f, 0.8f
};

inline struct glfwPointers
{
    Window* window;
    Camera* camera;
} glfwPtr;

constexpr GLfloat square[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f
};

constexpr GLuint squareIndices[6] = {
    0, 1, 3,
    1, 2, 3
};

constexpr glm::vec2 BOT_LEFT_BOUNDERY  = glm::vec2(square[0], square[1]);
constexpr glm::vec2 TOP_RIGHT_BOUNDERY = glm::vec2(square[6], square[7]);
