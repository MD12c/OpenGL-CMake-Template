#include "LUT.h"

#include "Globals.h"
#include "../Shaders/Shader.h"
#include "../Models/BasicShapes.h"

LUT::LUT(int width, int height)
    : tex(GL_RG16F, GL_RG, width, height)
{
    tex.Bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void LUT::Draw(ShaderID shaderID)
{
    GLint prevFramebuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);
    
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.ID, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "LUT framebuffer incomplete!" << std::endl;

    Shader::Activate(shaderID);
    
    tex.Bind(0);
    glClear(GL_COLOR_BUFFER_BIT);
    basicShapes->plane.DrawSimple(shaderID);
    
    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);
    glDeleteFramebuffers(1, &framebuffer);
}


void LUT::ExportUniformsTo(ShaderID shaderID)
{
    tex.texUnit(shaderID, "brdfLUT");
}