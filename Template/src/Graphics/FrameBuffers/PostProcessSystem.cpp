#include "PostProcessSystem.h"

#include "Globals.h"
#include "Square.h"

PostProcessSystem::PostProcessSystem()
    : antiAlias(),
      finalFrameBuffer(2, true),
      bloom()
{
}

PostProcessSystem::~PostProcessSystem()
{
}

void PostProcessSystem::Begin()
{
    antiAlias.Activate();
}

void PostProcessSystem::End(int postShaderID, int blurShaderID)
{
    antiAlias.CopyResultsTo(finalFrameBuffer.ID);
    GLuint blurredTexture = bloom.BlurPass(finalFrameBuffer.textureIDs[1], blurShaderID, 5);

    ShaderManager::Activate(postShaderID);
    glUniform1f(ShaderManager::getLoc(postShaderID, "gamma"), gamma);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, finalFrameBuffer.textureIDs[0]);
    glUniform1i(ShaderManager::getLoc(postShaderID, "tex0"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, blurredTexture);
    glUniform1i(ShaderManager::getLoc(postShaderID, "tex1"), 1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glDisable(GL_DEPTH_TEST);
    quad->DrawSquare();
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
}
