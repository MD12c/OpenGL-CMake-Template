#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include <glad/glad.h>

#include "..\GLObjects\Texture.h"
#include "..\GLObjects\RBO.h"

class Framebuffer
{
public:
    GLuint               ID;
    std::string          name;
    std::unique_ptr<RBO> framebufferRBO = nullptr;
    std::deque<Texture>  textures;

    int crntWidth = 0, crntHeight = 0;

    Framebuffer(int numRenderTargets, bool hasDepthStencil, int width, int height, const std::string& name);

    void Resize(int w, int h);
};

#endif