#include "Framebuffer.h"

#include "Globals.h"

Framebuffer::Framebuffer(int numRenderTargets, bool hasDepthStencil, const std::string& name)
    : framebufferRBO(hasDepthStencil ? std::make_unique<RBO>(false) : nullptr)
{
    GLint prevFramebuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);
    
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glObjectLabel(GL_FRAMEBUFFER, ID, -1, name.c_str());

    for (int i = 0; i < numRenderTargets; i++)
    {
        textures.emplace_back(GL_RGB16F, GL_RGB, width, height, GL_UNSIGNED_BYTE, nullptr, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i].ID, 0);
    }

    if (framebufferRBO)
    {
        framebufferRBO->Bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebufferRBO->ID);
    }

    std::vector<GLenum> attachments;
    attachments.reserve(numRenderTargets);
    for (int i = 0; i < numRenderTargets; i++)
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    glDrawBuffers(numRenderTargets, attachments.data());

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);
}

void Framebuffer::Resize(int w, int h)
{
    crntWidth  = w;
    crntHeight = h;

    for (const auto& tex : textures)
    {
        glBindTexture(GL_TEXTURE_2D, tex.ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }

    if (framebufferRBO)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, framebufferRBO->ID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    }
}