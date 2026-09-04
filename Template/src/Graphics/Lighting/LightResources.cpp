#include "LightResources.h"

#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

#include "Globals.h"
#include "../Renderer.h"
#include "../Materials/MaterialManager.h"
#include "../Models/BasicShapes.h"

LightResources::LightResources(float zNear, float zFar)
    : zNear(zNear),
      zFar(zFar),
      material2D(MaterialManager::LoadMaterialCustom("Shadow2D", ShaderID::SHADOW_MAP2D)),
      materialCube(MaterialManager::LoadMaterialCustom("ShadowCube", ShaderID::SHADOW_MAPCUBE)),
      materialSphere(MaterialManager::LoadMaterialCustom("lightSphere", ShaderID::LIGHT_SPHERE)),
      pointUBO(sizeof(PointLight::PointLightsUBO), GL_STATIC_DRAW),
      dirUBO(sizeof(DirectionLight::DirLightsUBO), GL_STATIC_DRAW),
      spotUBO(sizeof(SpotLight::SpotLightsUBO), GL_STATIC_DRAW)
{
    GLint prevFramebuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);

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

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf2D);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufCube);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);

    pointUBO.linkShader(ShaderID::PBR, "PointLightsBlock");
    dirUBO.linkShader(ShaderID::PBR, "DirLightsBlock");
    spotUBO.linkShader(ShaderID::PBR, "SpotLightsBlock");
}

LightResources::~LightResources()
{
    glDeleteTextures(1, &shadowMapDirArray);
    glDeleteTextures(1, &shadowMapSpotArray);
    glDeleteTextures(1, &shadowMapPointArray);
    glDeleteFramebuffers(1, &frameBuf2D);
    glDeleteFramebuffers(1, &frameBufCube);
}

void LightResources::genTexture(GLenum type, GLsizei maxDepth, GLuint* shadowMapTexture)
{
    glGenTextures(1, shadowMapTexture);
    glBindTexture(type, *shadowMapTexture);
    glTexImage3D(type, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, maxDepth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, clampColor);
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

void LightResources::DrawLightSpheres(ShaderID shaderID, glm::vec3 pos, glm::vec3 color) const
{
    Shader::Activate(shaderID);
    glUniform4fv(Shader::getLoc(shaderID, "lightColor"), 1, glm::value_ptr(color));
    basicShapes->icoSphere.Draw(shaderID, Transform(pos), nullptr, materialSphere);
}

void LightResources::DrawLightPlanes(ShaderID shaderID, glm::vec3 pos, glm::vec3 direction, glm::vec3 color) const
{
    Shader::Activate(shaderID);
    glm::vec4 col = glm::vec4(color, 1.0f);
    glUniform4fv(Shader::getLoc(shaderID, "lightColor"), 1, glm::value_ptr(col));
    glm::vec3 dir        = glm::normalize(direction);
    glm::vec3 defaultDir = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::quat rotation   = glm::rotation(defaultDir, dir);
    basicShapes->plane.Draw(shaderID, Transform(pos, rotation), nullptr, materialSphere);
}

void LightResources::ExportUniformsTo(ShaderID shaderID) const
{
    BindShadowTextures(shaderID);

    GLint numSpotLights  = static_cast<GLint>(spotLights.size());
    GLint numDirLights   = static_cast<GLint>(directionLights.size());
    GLint numPointLights = static_cast<GLint>(pointLights.size());

    if (DirectionLight::updateUBOdata)
    {
        DirectionLight::DirLightsUBO data{};
        data.numDirLights = numDirLights;
        for (int i = 0; i < numDirLights; ++i)
        {
            data.direction[i]    = glm::vec4(directionLights[i].getDirection(), 0.0f);
            data.color[i]        = glm::vec4(directionLights[i].getColor(), 0.0f);
            data.shadowMatrix[i] = directionLights[i].getShadowMatrix();
            data.layerIndex[i]   = glm::ivec4(directionLights[i].layerIndex);
        }
        dirUBO.Bind();
        dirUBO.UpdateData(&data, sizeof(DirectionLight::DirLightsUBO));
        DirectionLight::updateUBOdata = false;
    }

    if (SpotLight::updateUBOdata)
    {
        SpotLight::SpotLightsUBO data{};
        data.numSpotLights = numSpotLights;
        for (int i = 0; i < numSpotLights; ++i)
        {
            data.innerCone[i]    = glm::vec4(spotLights[i].innerCone);
            data.outerCone[i]    = glm::vec4(spotLights[i].outerCone);
            data.pos[i]          = glm::vec4(spotLights[i].getPosition(), 0.0f);
            data.direction[i]    = glm::vec4(spotLights[i].getDirection(), 0.0f);
            data.color[i]        = glm::vec4(spotLights[i].getColor(), 0.0f);
            data.shadowMatrix[i] = spotLights[i].getShadowMatrix();
            data.layerIndex[i]   = glm::ivec4(spotLights[i].layerIndex);
        }
        spotUBO.Bind();
        spotUBO.UpdateData(&data, sizeof(SpotLight::SpotLightsUBO));
        SpotLight::updateUBOdata = false;
    }

    if (PointLight::updateUBOdata)
    {
        PointLight::PointLightsUBO data{};
        data.numPointLights = numPointLights;
        for (int i = 0; i < data.numPointLights; ++i)
        {
            data.pos[i]        = glm::vec4(pointLights[i].getPosition(), 0.0f);
            data.color[i]      = glm::vec4(pointLights[i].getColor(), 0.0f);
            data.farPlane[i]   = glm::vec4(pointLights[i].getFarPlane());
            data.layerIndex[i] = glm::ivec4(pointLights[i].layerIndex);
        }
        pointUBO.Bind();
        pointUBO.UpdateData(&data, sizeof(PointLight::PointLightsUBO));
        PointLight::updateUBOdata = false;
    }
}

void LightResources::ShadowPass(Renderer* renderer, const std::vector<Model>& models, Transform transform) const
{
    InstrumentationTimer timerCPU("ShadowPass");
    renderer->BindFramebuffer(frameBuf2D);
    renderer->BindFramebuffer(frameBufCube);

    for (const auto& pointLight : pointLights)
    {
        ShaderID shaderID = ShaderID::SHADOW_MAPCUBE;
        pointLight.BeginDepthPass(shaderID);
        renderer->BindFramebuffer(frameBufCube);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapPointArray, 0);
        glClear(GL_DEPTH_BUFFER_BIT);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "pointLight FBO incomplete: " << status << std::endl;

        for (const auto& model : models)
            model.Draw(shaderID, transform, nullptr, materialCube);
    }

    for (const auto& directionLight : directionLights)
    {
        ShaderID shaderID = ShaderID::SHADOW_MAP2D;
        directionLight.BeginDepthPass(shaderID);
        renderer->BindFramebuffer(frameBuf2D);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapDirArray, 0, directionLight.layerIndex);
        glClear(GL_DEPTH_BUFFER_BIT);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "directionLight FBO incomplete: " << status << std::endl;

        for (const auto& model : models)
            model.Draw(shaderID, transform, nullptr, material2D);
    }

    for (const auto& spotLight : spotLights)
    {
        ShaderID shaderID = ShaderID::SHADOW_MAP2D;
        spotLight.BeginDepthPass(shaderID);
        renderer->BindFramebuffer(frameBuf2D);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapSpotArray, 0, spotLight.layerIndex);
        glClear(GL_DEPTH_BUFFER_BIT);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "spotLight FBO incomplete: " << status << std::endl;

        for (const auto& model : models)
            model.Draw(shaderID, transform, nullptr, material2D);
    }
}

void LightResources::addDirectionLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top)
{
    directionLights.emplace_back(static_cast<GLuint>(directionLights.size()), lightPos, direction, lightColor, left, right, bottom, top, zNear, zFar);
}

void LightResources::addSpotLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone)
{
    spotLights.emplace_back(static_cast<GLuint>(spotLights.size()), lightPos, direction, lightColor, fovDeg, innerCone, outerCone, zNear, zFar);
}

void LightResources::addPointLight(glm::vec3 lightPos, glm::vec3 lightColor)
{
    pointLights.emplace_back(static_cast<GLuint>(pointLights.size()), lightPos, lightColor, zNear, zFar);
}