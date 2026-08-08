#ifndef SHADOW_CASTER_CLASS_H
#define SHADOW_CASTER_CLASS_H

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "ShadowSystem.h"
#include "../../Shaders/ShaderManager.h"
#include "../../LightSystem.h"

class ShadowCaster
{
public:
    static constexpr unsigned int SHADOW_MAP_WIDTH  = 2048;
    static constexpr unsigned int SHADOW_MAP_HEIGHT = 2048;
    static constexpr float        clampColor[4]     = { 1.0f, 1.0f, 1.0f, 1.0f };

protected:
    LightSystem::LightType lightType;

public:
    virtual ~ShadowCaster() = default;

    virtual void setView(glm::vec3 newPosition, glm::vec3 newDirection)            = 0;
    virtual void BeginDepthPass(unsigned int shaderID, ShadowSystem& shadowSystem) = 0;
    virtual void ExportUniformsTo(unsigned int shaderID, int lightIndex)           = 0;

    inline void EndDepthPass() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    inline LightSystem::LightType getType() { return lightType; }
    unsigned int                  getShaderIDfromType(LightSystem::LightType type);
};

#endif