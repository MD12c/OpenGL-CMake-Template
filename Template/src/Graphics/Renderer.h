#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "Scene.h"
#include "Frustum/Frustum.h"
#include "FrameBuffers/MSAABuffer.h"
#include "FrameBuffers/Framebuffer.h"
#include "FrameBuffers/Bloom.h"
#include "FrameBuffers/LUT.h"
#include "Models/Transform.h"

enum class RenderFeature : uint8_t
{
    DEPTH,
    STENCIL,
    CULL,
    MULTISAMPLE,
    LAST
};

enum class DepthFunc : GLenum
{
    LESS   = GL_LESS,
    LEQUAL = GL_LEQUAL,
    GRATER = GL_GREATER,
    ALWAYS = GL_ALWAYS
};

enum class CullMode : GLenum
{
    FRONT          = GL_FRONT,
    BACK           = GL_BACK,
    FRONT_AND_BACK = GL_FRONT_AND_BACK
};

class Renderer
{
public:
    std::array<bool, static_cast<size_t>(RenderFeature::LAST)> enabledFeatures = {};

    DepthFunc currentDepthFunc = DepthFunc::LESS;
    CullMode  currentCullMode  = CullMode::BACK;

    MSAAbuffer  antiAlias;
    Framebuffer finalFrameBuffer;
    Bloom       bloom;
    LUT         lut;
    Frustum     frustum;

    std::shared_ptr<Texture> noTexture;

    GLuint boundFramebuffer = 0;
    void   BindFramebuffer(GLenum ID) { ID != boundFramebuffer ? glBindFramebuffer(GL_FRAMEBUFFER, boundFramebuffer = ID) : void(); }

    Renderer();
    ~Renderer();

    void Render(const Scene& scene);

private:
    void set(RenderFeature feature, bool enable);
    void setCullMode(CullMode mode);
    void setDepthFunc(DepthFunc func);
};

#endif