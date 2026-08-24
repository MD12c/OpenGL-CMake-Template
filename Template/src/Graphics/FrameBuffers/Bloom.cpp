#include "Bloom.h"

#include "Globals.h"
#include "../Shaders/ShaderManager.h"
#include "Square.h"

Bloom::Bloom()
    : buffer1(1, false),
      buffer2(1, false)
{
}

GLuint Bloom::BlurPass(GLuint brightTexture, ShaderIDs shaderID, int numPasses)
{
    ShaderManager::Activate(shaderID);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(ShaderManager::getLoc(shaderID, "tex0"), 0);
    glViewport(0, 0, width, height);

    bool horizontal = true, first_iteration = true;

    for (int i = 0; i < numPasses; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, horizontal ? buffer1.ID : buffer2.ID);
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform1i(ShaderManager::getLoc(shaderID, "horizontal"), horizontal);

        if (first_iteration)
            glBindTexture(GL_TEXTURE_2D, brightTexture);
        else
            glBindTexture(GL_TEXTURE_2D, horizontal ? buffer2.textureIDs[0] : buffer1.textureIDs[0]);

        quad->DrawSquare();
        horizontal      = !horizontal;
        first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return horizontal ? buffer2.textureIDs[0] : buffer1.textureIDs[0];
}

void Bloom::Resize(int w, int h)
{
    buffer1.Resize(w, h);
    buffer2.Resize(w, h);
}