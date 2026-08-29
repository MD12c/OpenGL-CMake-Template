#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

//#define BENCHMARK
#include "Benchmarks/Instrumentatior.h"
#include "Benchmarks/GPUtimer.h"


inline int         width        = 2560;
inline int         height       = 1440;
inline std::string windowName   = "Template";
inline GLfloat     gamma        = 2;
inline GLfloat     windowRGB[3] = {
    pow(0.8f, gamma), pow(0.8f, gamma), pow(0.8f, gamma)
};

class Window;
class Camera;
class Renderer;

inline struct glfwPointers
{
    Window*   window;
    Camera*   camera;
    Renderer* renderer;
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

constexpr GLuint numSamples = 4;

constexpr unsigned int MAX_DIR_LIGHTS   = 8;
constexpr unsigned int MAX_SPOT_LIGHTS  = 8;
constexpr unsigned int MAX_POINT_LIGHTS = 8;

enum LightType
{
    DIRECTION,
    SPOT,
    POINT
};

enum ShaderID
{
    SPECULAR,
    PBR,
    SKYBOX,
    POSTPROCESS,
    SHADOW_MAP2D,
    SHADOW_MAPCUBE,
    DEPTH_DEBUG2D,
    DEPTH_DEBUGCUBE,
    LIGHT_SPHERE,
    BLUR,
    HDR_CONVERTER,
    IRRADIANCE,
    PREFILTER,
    BRDF_LUT,
    LAST_SHADER
};

using MaterialID                        = uint32_t;
constexpr MaterialID NO_MATERIAL        = UINT32_MAX;
constexpr MaterialID USE_FILE_MATERIAL  = UINT32_MAX - 1;
constexpr MaterialID WIREFRAME_MATERIAL = UINT32_MAX - 2;

class Square;
inline Square* quad = nullptr;

struct DebugGroup
{
    DebugGroup(const char* name) { glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name); }
    ~DebugGroup() { glPopDebugGroup(); }
};

//-----------------------------------------------------------------------
// Temporary Globals Below
//-----------------------------------------------------------------------
