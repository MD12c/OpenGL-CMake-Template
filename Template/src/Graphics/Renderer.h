#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "Scene.h"
#include "Frustum/Frustum.h"
#include "FrameBuffers/MSAABuffer.h"
#include "FrameBuffers/Framebuffer.h"
#include "FrameBuffers/Bloom.h"
#include "FrameBuffers/LUT.h"
#include "Models/Transform.h"

// _b bool, _c choice
enum RenderFlag
{
    DEPTH_b,
    DEPTH_FUNC_c,
    STENCIL_b,
    COLOR_b,
    CULL_b,
    CULL_FACE_c,
    FRONT_FACE_c,
    MULTISAMPLE_b,
    LAST_RENDERFLAG
};

class Renderer
{
public:
    std::array<GLenum, RenderFlag::LAST_RENDERFLAG> flags = {};

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
    void        set(RenderFlag f, GLenum value);
    bool        isSet(RenderFlag f) const { return flags[f]; }
    static void applyFlag(bool apply, GLenum target) { apply ? glEnable(target) : glDisable(target); }
};

#endif