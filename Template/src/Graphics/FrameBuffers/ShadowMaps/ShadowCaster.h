#ifndef SHADOW_CASTER_CLASS_H
#define SHADOW_CASTER_CLASS_H

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "../../Shaders/ShaderManager.h"

class ShadowCaster
{
public:
    GLuint ID;

    static constexpr unsigned int MAX_DIR_LIGHTS   = 2;
    static constexpr unsigned int MAX_SPOT_LIGHTS  = 4;
    static constexpr unsigned int MAX_POINT_LIGHTS = 4;

    enum LightType
    {
        DIRECTION,
        SPOT,
        POINT
    };

protected:
    const unsigned int SHADOW_MAP_WIDTH  = 2048;
    const unsigned int SHADOW_MAP_HEIGHT = 2048;
    const float        clampColor[4]     = { 1.0f, 1.0f, 1.0f, 1.0f };

    LightType lightType;

public:
    virtual ~ShadowCaster() = default;

    virtual void setView(glm::vec3 newPosition, glm::vec3 newDirection)                      = 0;
    virtual void BeginDepthPass(unsigned int shaderID)                                       = 0;
    virtual void EndDepthPass()                                                              = 0;
    virtual void ExportUniformsTo(unsigned int shaderID, GLuint textureSlot, int lightIndex) = 0;

    inline LightType    getType() { return lightType; }
    unsigned int getShaderIDfromType(LightType type);
};

#endif