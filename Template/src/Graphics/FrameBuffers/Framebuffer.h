#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include <glad/glad.h>

#include "..\GLObjects\Texture.h"
#include "..\GLObjects\RBO.h"

class Framebuffer
{
public:
    GLuint               ID;
    std::unique_ptr<RBO> framebufferRBO = nullptr;
    std::vector<GLuint>  textureIDs;

    Framebuffer(int numRenderTargets, bool hasDepthStencil);

    void ClearBuffer();
    void Resize(int w, int h);
    void Activate();
};

#endif