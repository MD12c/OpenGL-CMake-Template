#include "LightSystem.h"

#include "FrameBuffers/ShadowMaps/ShadowCaster.h"
#include "FrameBuffers/ShadowMaps/ShadowSystem.h"
#include "FrameBuffers/ShadowMaps/ShadowMap2D.h"
#include "FrameBuffers/ShadowMaps/ShadowMapCube.h"

LightSystem::LightSystem(float zNear, float zFar)
    : shadowSystem(std::make_unique<ShadowSystem>()), zNear(zNear), zFar(zFar)
{
}

void LightSystem::ExportUniforms(unsigned int shaderID)
{
    shadowSystem->BindShadowTextures(shaderID, 2);

    int dirIdx = 0, spotIdx = 0, pointIdx = 0;
    for (auto& caster : shadowCasters)
    {
        switch (caster->getType())
        {
            case LightType::DIRECTION:
                caster->ExportUniformsTo(shaderID, dirIdx);
                dirIdx++;
                break;
            case LightType::SPOT:
                caster->ExportUniformsTo(shaderID, spotIdx);
                spotIdx++;
                break;
            case LightType::POINT:
                caster->ExportUniformsTo(shaderID, pointIdx);
                pointIdx++;
                break;
        }
    }
    glUniform1i(ShaderManager::getLoc(shaderID, "numDirLights"), dirIdx);
    glUniform1i(ShaderManager::getLoc(shaderID, "numSpotLights"), spotIdx);
    glUniform1i(ShaderManager::getLoc(shaderID, "numPointLights"), pointIdx);
}

void LightSystem::ShadowPass(Model& model)
{
    for (auto& caster : shadowCasters)
    {
        unsigned int shaderID = caster->getShaderIDfromType(caster->getType());
        caster->BeginDepthPass(shaderID, *shadowSystem);
        model.Draw(shaderID);
        caster->EndDepthPass();
    }
}

void LightSystem::addLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top)
{
    shadowCasters.emplace_back(
        std::make_unique<ShadowMap2D>(
            shadowSystem->RegisterCaster(LightType::DIRECTION),
            lightPos, direction, lightColor, left, right, bottom, top, zNear, zFar));
}

void LightSystem::addLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone)
{
    shadowCasters.emplace_back(
        std::make_unique<ShadowMap2D>(
            shadowSystem->RegisterCaster(LightType::SPOT),
            lightPos, direction, lightColor, fovDeg, innerCone, outerCone, zNear, zFar));
}

void LightSystem::addLight(glm::vec3 lightPos, glm::vec3 lightColor)
{
    shadowCasters.emplace_back(
        std::make_unique<ShadowMapCube>(
            shadowSystem->RegisterCaster(LightType::POINT),
            lightPos, lightColor, zNear, zFar));
}
