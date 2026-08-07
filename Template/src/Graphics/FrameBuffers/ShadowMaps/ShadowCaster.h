#ifndef SHADOW_CASTER_CLASS_H
#define SHADOW_CASTER_CLASS_H

#include <string>

#include "glad/glad.h"

class ShadowCaster
{
protected:
    const unsigned int SHADOW_MAP_WIDTH  = 2048;
    const unsigned int SHADOW_MAP_HEIGHT = 2048;
    const float        clampColor[4]     = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
    GLuint ID;

    virtual ~ShadowCaster() = default;

    virtual void BeginDepthPass(const std::string& shaderName)                       = 0;
    virtual void EndDepthPass()                                                      = 0;
    virtual void ExportUniformsTo(const std::string& shaderName, GLuint textureSlot) = 0;
};

#endif