#include "Framebuffer.h"

#include "Globals.h"
#include "../Shaders/ShaderManager.h"
#include "Square.h"

Framebuffer::Framebuffer(int numRenderTargets, bool hasDepthStencil)
    : framebufferRBO(hasDepthStencil ? std::make_unique<RBO>(false) : nullptr)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

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
}

void Framebuffer::Activate()
{
    glViewport(0, 0, width, height);
    ClearBuffer();

    if (framebufferRBO)
        framebufferRBO->Bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Framebuffer::ClearBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Framebuffer::Resize(int w, int h)
{
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