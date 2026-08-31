#include "LightResources.h"

#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

#include "Globals.h"
#include "../Renderer.h"
#include "ShadowCaster.h"
#include "ShadowMap2D.h"
#include "ShadowMapCube.h"
#include "../Materials/MaterialManager.h"
#include "../Models/BasicShapes.h"

LightResources::LightResources(float zNear, float zFar)
    : zNear(zNear),
      zFar(zFar),
      material2D(MaterialManager::LoadMaterialCustom("Shadow2D", ShaderID::SHADOW_MAP2D)),
      materialCube(MaterialManager::LoadMaterialCustom("ShadowCube", ShaderID::SHADOW_MAPCUBE)),
      materialSphere(MaterialManager::LoadMaterialCustom("lightSphere", ShaderID::LIGHT_SPHERE))
{
    glGenFramebuffers(1, &frameBuf2D);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf2D);
    glObjectLabel(GL_FRAMEBUFFER, frameBuf2D, -1, "ShadowMap FBO 2D");
    glGenFramebuffers(1, &frameBufCube);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufCube);
    glObjectLabel(GL_FRAMEBUFFER, frameBufCube, -1, "ShadowMap FBO Cube");

    genTexture(GL_TEXTURE_2D_ARRAY, MAX_DIR_LIGHTS, &shadowMapDirArray);
    glObjectLabel(GL_TEXTURE, shadowMapDirArray, -1, "ShadowMap Dir Array");
    genTexture(GL_TEXTURE_2D_ARRAY, MAX_SPOT_LIGHTS, &shadowMapSpotArray);
    glObjectLabel(GL_TEXTURE, shadowMapSpotArray, -1, "ShadowMap Spot Array");
    genTexture(GL_TEXTURE_CUBE_MAP_ARRAY, MAX_POINT_LIGHTS * 6, &shadowMapPointArray);
    glObjectLabel(GL_TEXTURE, shadowMapPointArray, -1, "ShadowMap Point Array");

    GLint prevFramebuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf2D);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufCube);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);
}

LightResources::~LightResources()
{
    glDeleteTextures(1, &shadowMapDirArray);
    glDeleteTextures(1, &shadowMapSpotArray);
    glDeleteTextures(1, &shadowMapPointArray);
    shadowMapDirArray   = 0;
    shadowMapSpotArray  = 0;
    shadowMapPointArray = 0;
}

void LightResources::genTexture(GLenum type, GLsizei maxDepth, GLuint* shadowMapTexture)
{
    glGenTextures(1, shadowMapTexture);
    glBindTexture(type, *shadowMapTexture);
    glTexImage3D(type, 0, GL_DEPTH_COMPONENT,
                 ShadowCaster::SHADOW_MAP_WIDTH,
                 ShadowCaster::SHADOW_MAP_HEIGHT,
                 maxDepth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, ShadowCaster::clampColor);
}

void LightResources::BindShadowTextures(ShaderID shaderID) const
{
    Shader::Activate(shaderID);

    GLint texDir = Shader::getUnit(shaderID, "dirShadowMaps");
    glActiveTexture(GL_TEXTURE0 + texDir);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapDirArray);
    glUniform1i(Shader::getLoc(shaderID, "dirShadowMaps"), texDir);

    GLint texSpot = Shader::getUnit(shaderID, "spotShadowMaps");
    glActiveTexture(GL_TEXTURE0 + texSpot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapSpotArray);
    glUniform1i(Shader::getLoc(shaderID, "spotShadowMaps"), texSpot);

    GLint texPoint = Shader::getUnit(shaderID, "pointShadowMaps");
    glActiveTexture(GL_TEXTURE0 + texPoint);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, shadowMapPointArray);
    glUniform1i(Shader::getLoc(shaderID, "pointShadowMaps"), texPoint);
}

GLint LightResources::RegisterCaster(LightType type)
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

void LightResources::DrawLightSpheres(ShaderID shaderID, size_t index) const
{
    Shader::Activate(shaderID);
    glm::vec4 col = glm::vec4(lights.at(index).getColor(), 1.0f);
    glUniform4fv(Shader::getLoc(shaderID, "lightColor"), 1, glm::value_ptr(col));
    basicShapes->icoSphere.Draw(shaderID, Transform(lights.at(index).getPosition()), nullptr, materialSphere);
}

void LightResources::DrawLightPlanes(ShaderID shaderID, size_t index) const
{
    Shader::Activate(shaderID);
    glm::vec4 col = glm::vec4(lights.at(index).getColor(), 1.0f);
    glUniform4fv(Shader::getLoc(shaderID, "lightColor"), 1, glm::value_ptr(col));
    glm::vec3 dir        = glm::normalize(lights.at(index).getDirection());
    glm::vec3 defaultDir = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::quat rotation   = glm::rotation(defaultDir, dir);
    basicShapes->plane.Draw(shaderID, Transform(lights.at(index).getPosition(), rotation), nullptr, materialSphere);
}

void LightResources::ExportUniformsTo(ShaderID shaderID) const
{
    BindShadowTextures(shaderID);

    int dirIdx = 0, spotIdx = 0, pointIdx = 0;
    for (auto& light : lights)
    {
        switch (light.getType())
        {
            case LightType::DIRECTION:
                light.caster->ExportUniformsTo(shaderID, dirIdx, light.getPosition(), light.getDirection(), light.getColor());
                dirIdx++;
                break;
            case LightType::SPOT:
                light.caster->ExportUniformsTo(shaderID, spotIdx, light.getPosition(), light.getDirection(), light.getColor());
                spotIdx++;
                break;
            case LightType::POINT:
                light.caster->ExportUniformsTo(shaderID, pointIdx, light.getPosition(), light.getDirection(), light.getColor());
                pointIdx++;
                break;
        }
    }
    glUniform1i(Shader::getLoc(shaderID, "numDirLights"), dirIdx);
    glUniform1i(Shader::getLoc(shaderID, "numSpotLights"), spotIdx);
    glUniform1i(Shader::getLoc(shaderID, "numPointLights"), pointIdx);
}

void LightResources::ShadowPass(Renderer* renderer, const std::vector<Model>& models, Transform transform) const
{
    InstrumentationTimer timerCPU("ShadowPass");
    renderer->BindFramebuffer(frameBuf2D);
    renderer->BindFramebuffer(frameBufCube);

    for (const auto& light : lights)
    {
        LightType  lightType = light.getType();
        ShaderID   shaderID  = Shader::getShaderIDfromLightType(lightType);
        MaterialID materialID;
        light.caster->BeginDepthPass(shaderID, light.getPosition());

        if (lightType == POINT)
        {
            materialID = materialCube;
            renderer->BindFramebuffer(frameBufCube);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapPointArray, 0);
        }
        else
        {
            materialID = material2D;
            renderer->BindFramebuffer(frameBuf2D);
            glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                      lightType == DIRECTION ? shadowMapDirArray : shadowMapSpotArray,
                                      0, light.caster->layerIndex);
        }
        glClear(GL_DEPTH_BUFFER_BIT);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ShadowMap FBO incomplete: " << status << std::endl;

        for (const auto& model : models)
            model.Draw(shaderID, transform, nullptr, materialID);
    }
}

void LightResources::addDirectionLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top)
{
    auto caster = std::make_unique<ShadowMap2D>(RegisterCaster(LightType::DIRECTION), lightPos, direction, left, right, bottom, top, zNear, zFar);
    lights.emplace_back(LightType::DIRECTION, lightPos, direction, lightColor, std::move(caster));
}

void LightResources::addSpotLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone)
{
    auto caster = std::make_unique<ShadowMap2D>(RegisterCaster(LightType::SPOT), lightPos, direction, fovDeg, innerCone, outerCone, zNear, zFar);
    lights.emplace_back(LightType::SPOT, lightPos, direction, lightColor, std::move(caster));
}

void LightResources::addPointLight(glm::vec3 lightPos, glm::vec3 lightColor)
{
    auto caster = std::make_unique<ShadowMapCube>(RegisterCaster(LightType::POINT), lightPos, zNear, zFar);
    lights.emplace_back(LightType::POINT, lightPos, glm::vec3(0.0f), lightColor, std::move(caster));
}