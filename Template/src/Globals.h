#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

// #define BENCHMARK
#include "Benchmarks/Instrumentatior.h"
#include "Benchmarks/GPUtimer.h"

constexpr GLuint SHADOW_MAP_WIDTH  = 2048;
constexpr GLuint SHADOW_MAP_HEIGHT = 2048;

class Window;
class Scene;
class Renderer;

struct glfwPointers
{
    Window*   window   = nullptr;
    Scene*    scene    = nullptr;
    Renderer* renderer = nullptr;
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
    // DEFAULT,
    LAST_SHADER
};

using MaterialID                        = uint32_t;
constexpr MaterialID NO_MATERIAL        = UINT32_MAX;
constexpr MaterialID USE_FILE_MATERIAL  = UINT32_MAX - 1;
constexpr MaterialID WIREFRAME_MATERIAL = UINT32_MAX - 2;

class BasicShapes;
inline BasicShapes* basicShapes = nullptr;

struct DebugGroup
{
    DebugGroup(const char* name) { glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name); }
    ~DebugGroup() { glPopDebugGroup(); }
};

//-----------------------------------------------------------------------
// Temporary Globals Below
//-----------------------------------------------------------------------
