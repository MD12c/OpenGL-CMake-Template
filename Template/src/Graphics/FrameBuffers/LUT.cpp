#include "LUT.h"

#include "Globals.h"
#include "../Shaders/ShaderManager.h"
#include "Square.h"

LUT::LUT()
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
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.ID, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "LUT framebuffer incomplete!" << std::endl;

    ShaderManager::Activate(ShaderID::BRDF_LUT);
    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    
    tex.Bind(0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    quad->DrawSquare();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
    glDeleteFramebuffers(1, &framebuffer);
}


void LUT::ExportUniformsTo(ShaderID shaderID)
{
    ShaderManager::Activate(shaderID);
    tex.texUnit(shaderID, "brdfLUT");
}