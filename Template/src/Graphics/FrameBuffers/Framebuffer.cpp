#include "Framebuffer.h"

#include "Globals.h"
#include "../Shaders/ShaderManager.h"
#include "Square.h"

Framebuffer::Framebuffer(int numRenderTargets, bool hasDepthStencil)
    : framebufferRBO(hasDepthStencil ? std::make_unique<RBO>(false) : nullptr),
      textureIDs(numRenderTargets)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    for (int i = 0; i < numRenderTargets; i++)
    {
        glGenTextures(1, &textureIDs[i]);
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureIDs[i], 0);
    }

    if (framebufferRBO)
    {
        framebufferRBO->Bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebufferRBO->ID);
    }

    std::vector<GLenum> attachments;
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
    for (const auto& tex : textureIDs)
    {
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }

    if (framebufferRBO)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, framebufferRBO->ID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    }
}