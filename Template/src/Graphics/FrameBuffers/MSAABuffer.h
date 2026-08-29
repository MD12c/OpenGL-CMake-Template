#ifndef MSAA_BUFFER_CLASS_H
#define MSAA_BUFFER_CLASS_H

#include <glad/glad.h>

#include "..\GLObjects\RBO.h"

#include "Framebuffer.h"

class MSAAbuffer
{
public:
    GLuint ID;
    RBO    MSAAbufferRBO;
    GLuint textureID;
    GLuint bloomTextureID;

    MSAAbuffer();
    void Resize(int w, int h);
    void CopyResultsTo(const Framebuffer& postProcessing);
};

#endif