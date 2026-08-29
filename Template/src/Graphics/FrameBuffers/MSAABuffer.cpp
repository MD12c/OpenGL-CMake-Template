#include "MSAAbuffer.h"
#include "Globals.h"

MSAAbuffer::MSAAbuffer()
    : MSAAbufferRBO(true)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glObjectLabel(GL_FRAMEBUFFER, ID, -1, "MSAA");

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
    glObjectLabel(GL_FRAMEBUFFER, ID, -1, "MSAA Texture");
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, GL_RGB16F, width, height, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureID, 0);

    glGenTextures(1, &bloomTextureID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, bloomTextureID);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, GL_RGB16F, width, height, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, bloomTextureID, 0);

    GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    MSAAbufferRBO.Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, MSAAbufferRBO.ID);

    auto MSAAStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (MSAAStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "MSAAbuffer error: " << MSAAStatus << std::endl;
}

void MSAAbuffer::CopyResultsTo(const Framebuffer& postProcessing)
{
    GPUInstrumentationTimer timer("MSAA copy");
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessing.ID);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void MSAAbuffer::Resize(int w, int h)
{
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, GL_RGB16F, w, h, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, bloomTextureID);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, GL_RGB16F, w, h, GL_TRUE);

    MSAAbufferRBO.Bind();
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, GL_DEPTH24_STENCIL8, w, h);
}