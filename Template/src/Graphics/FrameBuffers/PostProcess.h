#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include <glad/glad.h>

#include "../VertexBuffers/VAO.h"
#include "../VertexBuffers/VBO.h"
#include "../VertexBuffers/EBO.h"
#include "Texture.h"
#include "RBO.h"

class Framebuffer
{
public:
    GLuint ID;
    VAO    framebufferVAO;
    VBO    framebufferVBO;
    EBO    framebufferEBO;
    RBO    framebufferRBO;
    GLuint textureID;

    Framebuffer();
    void ClearBuffer();
    void Resize(int w, int h);
    void Activate();
    void Draw(unsigned int shaderID);
};

#endif