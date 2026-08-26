#include "LightSystem.h"

#include "../Shaders/ShaderManager.h"
#include "../Materials/MaterialManager.h"

#include "ShadowCaster.h"
#include "ShadowSystem.h"
#include "ShadowMap2D.h"
#include "ShadowMapCube.h"

LightSystem::LightSystem(float zNear, float zFar)
    : shadowSystem(std::make_unique<ShadowSystem>()),
      zNear(zNear),
      zFar(zFar),
      icoSphere("Assets/Models/icoSphere.obj"),
      material2D(MaterialManager::LoadMaterialCustom("Shadow2D", ShaderID::SHADOW_MAP2D)),
      materialCube(MaterialManager::LoadMaterialCustom("ShadowCube", ShaderID::SHADOW_MAPCUBE)),
      materialSphere(MaterialManager::LoadMaterialCustom("lightSphere", ShaderID::LIGHT_SPHERE))
{
}

void LightSystem::DrawLightSpheres(ShaderID shaderID, const std::vector<Light>& lights) const
{
    ShaderManager::Activate(shaderID);
    glEnable(GL_CULL_FACE);
    for (const auto& light : lights)
    {
        glm::vec4 col = glm::vec4(light.getColor(), 1.0f);
        glUniform4fv(ShaderManager::getLoc(shaderID, "lightColor"), 1, glm::value_ptr(col));
        icoSphere.Draw(shaderID, Transform(light.getPosition()), materialSphere);
    }
    glDisable(GL_CULL_FACE);
}

void LightSystem::ExportUniforms(ShaderID shaderID, const std::vector<Light>& lights) const
{
    shadowSystem->BindShadowTextures(shaderID);

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
    glUniform1i(ShaderManager::getLoc(shaderID, "numDirLights"), dirIdx);
    glUniform1i(ShaderManager::getLoc(shaderID, "numSpotLights"), spotIdx);
    glUniform1i(ShaderManager::getLoc(shaderID, "numPointLights"), pointIdx);
}

void LightSystem::ShadowPass(const std::vector<Model>& models, const std::vector<Light>& lights) const
{
    shadowSystem->ClearAllTargets();

    for (const auto& light : lights)
    {
        LightType  type     = light.getType();
        ShaderID   shaderID = ShaderManager::getShaderIDfromLightType(type);
        MaterialID materialID;
        if (type == POINT)
            materialID = materialCube;
        else
            materialID = material2D;

        light.caster->BeginDepthPass(shaderID, *shadowSystem, light.getPosition());
        for (const auto& model : models)
            model.Draw(shaderID, {{}, {}, glm::vec3(0.02f)}, materialID);
        light.caster->EndDepthPass();
    }
}

void LightSystem::addDirectionLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top)
{
    auto caster = std::make_unique<ShadowMap2D>(
        shadowSystem->RegisterCaster(LightType::DIRECTION),
        lightPos, direction, left, right, bottom, top, zNear, zFar);
    lights.emplace_back(LightType::DIRECTION, lightPos, direction, lightColor, std::move(caster));
}

void LightSystem::addSpotLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone)
{
    auto caster = std::make_unique<ShadowMap2D>(
        shadowSystem->RegisterCaster(LightType::SPOT),
        lightPos, direction, fovDeg, innerCone, outerCone, zNear, zFar);
    lights.emplace_back(LightType::SPOT, lightPos, direction, lightColor, std::move(caster));
}

void LightSystem::addPointLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 lightColor)
{
    auto caster = std::make_unique<ShadowMapCube>(
        shadowSystem->RegisterCaster(LightType::POINT),
        lightPos, zNear, zFar);
    lights.emplace_back(LightType::POINT, lightPos, glm::vec3(0.0f), lightColor, std::move(caster));
}