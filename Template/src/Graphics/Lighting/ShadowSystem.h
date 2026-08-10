#ifndef SHADOW_SYSTEM_CLASS_H
#define SHADOW_SYSTEM_CLASS_H

#include "glad/glad.h"

#include "Globals.h"

class ShadowSystem
{
private:
    int nextDirLayer   = 0;
    int nextSpotLayer  = 0;
    int nextPointLayer = 0;

public:
    GLuint shadowMapDirArray;
    GLuint shadowMapSpotArray;
    GLuint shadowMapPointArray;

    GLuint frameBuf2D;
    GLuint frameBufCube;

    static constexpr unsigned int MAX_DIR_LIGHTS   = 2;
    static constexpr unsigned int MAX_SPOT_LIGHTS  = 4;
    static constexpr unsigned int MAX_POINT_LIGHTS = 4;

    ShadowSystem();
    ~ShadowSystem();

    GLint RegisterCaster(LightType type);
    void  BindDepthTarget(LightType type, GLuint layerIndex);
    void  BindShadowTextures(unsigned int shaderID, GLuint startSlot);
    void  ShadowPass();

    void genTexture(GLenum type, GLsizei maxDepth, GLuint* shadowMapTexture);
    void attachTexture(GLuint frameBuf, GLuint texture, GLuint layer);
    void attachTextureLayered(GLuint frameBuf, GLuint texture);
    void setupFBO(GLuint frameBuf);
};

#endif