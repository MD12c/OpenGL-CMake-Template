#include "PostProcessSystem.h"

#include "Globals.h"
#include "Square.h"
#include "../Shaders/ShaderManager.h"

PostProcessSystem::PostProcessSystem()
    : antiAlias(),
      finalFrameBuffer(2, true),
      bloom(),
      lut()
{
    quad = new Square();
}

PostProcessSystem::~PostProcessSystem()
{
    delete quad;
}

void PostProcessSystem::Begin()
{
    antiAlias.Activate();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glDisable(GL_CULL_FACE);
    // glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void PostProcessSystem::End()
{
    antiAlias.CopyResultsTo(finalFrameBuffer);
    Texture& blurredTexture = bloom.BlurPass(finalFrameBuffer.textures[1].ID, ShaderID::BLUR, 5);

    ShaderManager::Activate(ShaderID::POSTPROCESS);
    glUniform1f(ShaderManager::getLoc(ShaderID::POSTPROCESS, "gamma"), gamma);
    finalFrameBuffer.textures[0].texUnit(ShaderID::POSTPROCESS, "tex0");
    blurredTexture.texUnit(ShaderID::POSTPROCESS, "tex1");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glDisable(GL_DEPTH_TEST);
    quad->DrawSquare();
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
}
