#include "Bloom.h"

#include "Globals.h"
#include "../Shaders/Shader.h"
#include "Square.h"

Bloom::Bloom()
    : buffer1(1, false, "Bloom1"),
      buffer2(1, false, "Bloom2")
{
}

Texture& Bloom::BlurPass(const Texture& brightTexture, ShaderID shaderID, int numPasses)
{
    GPUInstrumentationTimer timer("Blur Pass");
    Shader::Activate(shaderID);
    
    GLint prevFramebuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);
    glViewport(0, 0, width, height);

    bool horizontal = true, first_iteration = true;
    
    for (int i = 0; i < numPasses; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, horizontal ? buffer1.ID : buffer2.ID);
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform1i(Shader::getLoc(shaderID, "horizontal"), horizontal);

        if (first_iteration)
            brightTexture.Bind(0);
        else
            horizontal ? buffer2.textures[0].texUnit(shaderID, "tex0") : buffer1.textures[0].texUnit(shaderID, "tex0");

        quad->DrawSquare();
        horizontal      = !horizontal;
        first_iteration = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);
    return horizontal ? buffer2.textures[0] : buffer1.textures[0];
}

void Bloom::Resize(int w, int h)
{
    buffer1.Resize(w, h);
    buffer2.Resize(w, h);
}