#ifndef SHADOW_CASTER_CLASS_H
#define SHADOW_CASTER_CLASS_H

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Globals.h"

class ShadowCaster
{
public:
    GLuint layerIndex;

    static constexpr unsigned int SHADOW_MAP_WIDTH  = 2048;
    static constexpr unsigned int SHADOW_MAP_HEIGHT = 2048;
    static constexpr float        clampColor[4]     = { 1.0f, 1.0f, 1.0f, 1.0f };

protected:
    const LightType lightType;

public:
    ShadowCaster(LightType lightType, GLuint layerIndex) : lightType(lightType), layerIndex(layerIndex) {};
    virtual ~ShadowCaster() = default;

    virtual void setView(glm::vec3 newPosition, glm::vec3 newDirection)                                                                  = 0;
    virtual void BeginDepthPass(ShaderID shaderID, glm::vec3 lightPos)                                                                   = 0;
    virtual void ExportUniformsTo(ShaderID shaderID, int lightIndex, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 lightColor) = 0;
};

#endif