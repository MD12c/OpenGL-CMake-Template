#ifndef MSAA_BUFFER_CLASS_H
#define MSAA_BUFFER_CLASS_H

#include <glad/glad.h>

#include "..\GLObjects\VAO.h"
#include "..\GLObjects\VBO.h"
#include "..\GLObjects\EBO.h"
#include "..\GLObjects\Texture.h"
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