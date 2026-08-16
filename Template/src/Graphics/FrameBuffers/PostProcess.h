#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include <glad/glad.h>

#include "..\GLObjects\VAO.h"
#include "..\GLObjects\VBO.h"
#include "..\GLObjects\EBO.h"
#include "..\GLObjects\Texture.h"
#include "..\GLObjects\RBO.h"

class Framebuffer
{
public:
    GLuint       ID;
    int shaderID;
    VAO          framebufferVAO;
    VBO          framebufferVBO;
    EBO          framebufferEBO;
    RBO          framebufferRBO;
    GLuint       textureID;

    Framebuffer();
    void ClearBuffer();
    void Resize(int w, int h);
    void Activate();
    void Draw(int shaderID);
};

#endif