#ifndef SHADOW_CASTER_CLASS_H
#define SHADOW_CASTER_CLASS_H

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Globals.h"
#include "ShadowSystem.h"

class ShadowCaster
{
public:
    static constexpr unsigned int SHADOW_MAP_WIDTH  = 2048;
    static constexpr unsigned int SHADOW_MAP_HEIGHT = 2048;
    static constexpr float        clampColor[4]     = { 1.0f, 1.0f, 1.0f, 1.0f };

protected:
    const LightType lightType;

public:
    ShadowCaster(LightType lightType) : lightType(lightType) {};
    virtual ~ShadowCaster() = default;

    virtual void setView(glm::vec3 newPosition, glm::vec3 newDirection)                                                             = 0;
    virtual void BeginDepthPass(int shaderID, ShadowSystem& shadowSystem, glm::vec3 lightPos)                                       = 0;
    virtual void ExportUniformsTo(int shaderID, int lightIndex, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 lightColor) = 0;

    inline void EndDepthPass() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
};

#endif