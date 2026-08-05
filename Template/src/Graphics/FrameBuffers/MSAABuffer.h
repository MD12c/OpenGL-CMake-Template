#ifndef MSAA_BUFFER_CLASS_H
#define MSAA_BUFFER_CLASS_H

#include <glad/glad.h>

#include "../VertexBuffers/VAO.h"
#include "../VertexBuffers/VBO.h"
#include "../VertexBuffers/EBO.h"
#include "Texture.h"
#include "RBO.h"

class MSAAbuffer
{
public:
    GLuint ID;
    RBO    MSAAbufferRBO;
    GLuint textureID;

    MSAAbuffer();
    void Resize(int w, int h);
    void Activate();
    void CopyResultsTo(GLuint postProcessing);
};

#endif