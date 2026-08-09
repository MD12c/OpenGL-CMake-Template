#include "LightSystem.h"

#include "Shaders/ShaderManager.h"

#include "FrameBuffers/ShadowMaps/ShadowCaster.h"
#include "FrameBuffers/ShadowMaps/ShadowSystem.h"
#include "FrameBuffers/ShadowMaps/ShadowMap2D.h"
#include "FrameBuffers/ShadowMaps/ShadowMapCube.h"

LightSystem::LightSystem(float zNear, float zFar)
    : shadowSystem(std::make_unique<ShadowSystem>()), zNear(zNear), zFar(zFar), icoSphere("Assets/Models/crow.obj")
{
}

void LightSystem::RenderLightModels(unsigned int shaderID)
{
    ShaderManager::Activate(shaderID);
    icoSphere.Draw(shaderID);
}

void LightSystem::ExportUniforms(unsigned int shaderID)
{
    shadowSystem->BindShadowTextures(shaderID, 2 /*start slot*/);

    int dirIdx = 0, spotIdx = 0, pointIdx = 0;
    for (auto& light : lights)
    {
        switch (light.type)
        {
            case LightType::DIRECTION:
                light.caster->ExportUniformsTo(shaderID, dirIdx, light.pos, light.dir, light.color);
                dirIdx++;
                break;
            case LightType::SPOT:
                light.caster->ExportUniformsTo(shaderID, spotIdx, light.pos, light.dir, light.color);
                spotIdx++;
                break;
            case LightType::POINT:
                light.caster->ExportUniformsTo(shaderID, pointIdx, light.pos, light.dir, light.color);
                pointIdx++;
                break;
        }
    }
    glUniform1i(ShaderManager::getLoc(shaderID, "numDirLights"), dirIdx);
    glUniform1i(ShaderManager::getLoc(shaderID, "numSpotLights"), spotIdx);
    glUniform1i(ShaderManager::getLoc(shaderID, "numPointLights"), pointIdx);
}

unsigned int LightSystem::getShaderIDfromType(LightType type)
{
    if (type == LightType::DIRECTION)
        return ShaderManager::IDs.shadowMap2D;
    else if (type == LightType::SPOT)
        return ShaderManager::IDs.shadowMap2D;
    else if (type == LightType::POINT)
        return ShaderManager::IDs.shadowMapCube;
    else
        throw std::runtime_error("[ERROR] Invalid light type");
};

void LightSystem::ShadowPass(Model& model)
{
    for (auto& light : lights)
    {
        unsigned int shaderID = getShaderIDfromType(light.type);
        light.caster->BeginDepthPass(shaderID, *shadowSystem, light.pos);
        model.Draw(shaderID);
        light.caster->EndDepthPass();
    }
}

void LightSystem::addLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top)
{
    auto caster = std::make_unique<ShadowMap2D>(
        shadowSystem->RegisterCaster(LightType::DIRECTION),
        lightPos, direction, left, right, bottom, top, zNear, zFar);
    lights.emplace_back(LightType::DIRECTION, lightPos, direction, lightColor, std::move(caster));
}

void LightSystem::addLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone)
{
    auto caster = std::make_unique<ShadowMap2D>(
        shadowSystem->RegisterCaster(LightType::SPOT),
        lightPos, direction, fovDeg, innerCone, outerCone, zNear, zFar);
    lights.emplace_back(LightType::SPOT, lightPos, direction, lightColor, std::move(caster));
}

void LightSystem::addLight(glm::vec3 lightPos, glm::vec3 lightColor)
{
    auto caster = std::make_unique<ShadowMapCube>(
        shadowSystem->RegisterCaster(LightType::POINT),
        lightPos, zNear, zFar);
    lights.emplace_back(LightType::POINT, lightPos, glm::vec3(0.0f), lightColor, std::move(caster));
}