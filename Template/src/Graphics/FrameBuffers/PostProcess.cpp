#include "PostProcess.h"
#include "Globals.h"
#include "../Shaders/ShaderManager.h"

Framebuffer::Framebuffer()
    : framebufferVAO(),
      framebufferVBO(square, sizeof(square), GL_STATIC_DRAW),
      framebufferEBO(squareIndices, sizeof(squareIndices)),
      framebufferRBO(false)
{
    glfwPtr.framebuffer = this;
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    framebufferVAO.Bind();
    framebufferVBO.Bind();
    framebufferVAO.LinkAttrib(framebufferVBO, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void*)0);
    framebufferVAO.LinkAttrib(framebufferVBO, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

    framebufferRBO.Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebufferRBO.ID);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;
}

void Framebuffer::Activate()
{
    glfwPtr.framebuffer = this;
    glViewport(0, 0, width, height);
    ClearBuffer();
    framebufferRBO.Bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Framebuffer::Draw(unsigned int shaderID)
{
    glfwPtr.framebuffer = this;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ShaderManager::Activate(shaderID);
    framebufferVAO.Bind();
    framebufferVBO.Bind();
    framebufferEBO.Bind();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    framebufferVAO.Unbind();
    framebufferVBO.Unbind();
    framebufferEBO.Unbind();
}

void Framebuffer::ClearBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Framebuffer::Resize(int w, int h)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, framebufferRBO.ID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
}