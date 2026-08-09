#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

inline int         width        = 2560;
inline int         height       = 1440;
inline std::string windowName   = "Template";
inline GLfloat     gamma        = 2;
inline GLfloat     windowRGB[3] = {
    pow(0.8f, gamma), pow(0.8f, gamma), pow(0.8f, gamma)
};

class Window;
class Camera;
class Framebuffer;
class MSAAbuffer;

inline struct glfwPointers
{
    Window*      window;
    Camera*      camera;
    Framebuffer* framebuffer;
    MSAAbuffer*  msaabuffer;
} glfwPtr;

constexpr GLfloat square[20] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f
};

constexpr GLuint squareIndices[6] = {
    3, 1, 0,
    3, 2, 1
};

constexpr glm::vec2 BOT_LEFT_BOUNDERY  = glm::vec2(square[0], square[1]);
constexpr glm::vec2 TOP_RIGHT_BOUNDERY = glm::vec2(square[6], square[7]);

constexpr GLuint numSamples = 8;

enum LightType
{
    DIRECTION,
    SPOT,
    POINT
};

//-----------------------------------------------------------------------
// Temporary Globals Below
//-----------------------------------------------------------------------
