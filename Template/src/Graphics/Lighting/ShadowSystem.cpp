#include "ShadowSystem.h"

#include <iostream>

#include "Globals.h"
#include "../Shaders/ShaderManager.h"

#include "ShadowCaster.h"

ShadowSystem::ShadowSystem()
{
    glGenFramebuffers(1, &frameBuf2D);
    glGenFramebuffers(1, &frameBufCube);

    genTexture(GL_TEXTURE_2D_ARRAY, MAX_DIR_LIGHTS, &shadowMapDirArray);
    genTexture(GL_TEXTURE_2D_ARRAY, MAX_SPOT_LIGHTS, &shadowMapSpotArray);
    genTexture(GL_TEXTURE_CUBE_MAP_ARRAY, MAX_POINT_LIGHTS * 6, &shadowMapPointArray);

    setupFBO(frameBuf2D);
    setupFBO(frameBufCube);
}

void ShadowSystem::genTexture(GLenum type, GLsizei maxDepth, GLuint* shadowMapTexture)
{
    glGenTextures(1, shadowMapTexture);
    glBindTexture(type, *shadowMapTexture);
    glTexImage3D(type, 0, GL_DEPTH_COMPONENT,
                 ShadowCaster::SHADOW_MAP_WIDTH,
                 ShadowCaster::SHADOW_MAP_HEIGHT,
                 maxDepth,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, ShadowCaster::clampColor);
}

void ShadowSystem::setupFBO(GLuint frameBuf)
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowSystem::BindDepthTarget(LightType type, GLuint layerIndex)
{
    if (type == LightType::DIRECTION)
        attachTexture(frameBuf2D, shadowMapDirArray, layerIndex);
    else if (type == LightType::SPOT)
        attachTexture(frameBuf2D, shadowMapSpotArray, layerIndex);
    else if (type == LightType::POINT)
        attachTextureLayered(frameBufCube, shadowMapPointArray);
}

void ShadowSystem::attachTexture(GLuint frameBuf, GLuint texture, GLuint layer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0, layer);

    // GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    // if (status != GL_FRAMEBUFFER_COMPLETE)
    //     std::cout << "ShadowMap2D FBO incomplete: " << status << std::endl;
}

void ShadowSystem::attachTextureLayered(GLuint frameBuf, GLuint texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

    // GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    // if (status != GL_FRAMEBUFFER_COMPLETE)
    //     std::cout << "ShadowMapCube FBO incomplete: " << status << std::endl;
}

void ShadowSystem::ClearAllTargets()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf2D);
    glClear(GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufCube);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowSystem::BindShadowTextures(int shaderID, GLuint startSlot)
{
    ShaderManager::Activate(shaderID);

    glActiveTexture(GL_TEXTURE0 + startSlot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapDirArray);
    glUniform1i(ShaderManager::getLoc(shaderID, "dirShadowMaps"), startSlot);

    glActiveTexture(GL_TEXTURE0 + startSlot + 1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapSpotArray);
    glUniform1i(ShaderManager::getLoc(shaderID, "spotShadowMaps"), startSlot + 1);

    glActiveTexture(GL_TEXTURE0 + startSlot + 2);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, shadowMapPointArray);
    glUniform1i(ShaderManager::getLoc(shaderID, "pointShadowMaps"), startSlot + 2);
}

GLint ShadowSystem::RegisterCaster(LightType type)
{
    switch (type)
    {
        case LightType::DIRECTION:
            if (nextDirLayer >= MAX_DIR_LIGHTS) return -1;
            return nextDirLayer++;

        case LightType::SPOT:
            if (nextSpotLayer >= MAX_SPOT_LIGHTS) return -1;
            return nextSpotLayer++;

        case LightType::POINT:
            if (nextPointLayer >= MAX_POINT_LIGHTS) return -1;
            return nextPointLayer++;

        default:
            return -1;
    }
}

ShadowSystem::~ShadowSystem()
{
    glDeleteTextures(1, &shadowMapDirArray);
    glDeleteTextures(1, &shadowMapSpotArray);
    glDeleteTextures(1, &shadowMapPointArray);
    shadowMapDirArray   = 0;
    shadowMapSpotArray  = 0;
    shadowMapPointArray = 0;
}
