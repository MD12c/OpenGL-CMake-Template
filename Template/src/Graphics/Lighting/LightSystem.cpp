#include "LightSystem.h"

#include "../Shaders/ShaderManager.h"

#include "ShadowCaster.h"
#include "ShadowSystem.h"
#include "ShadowMap2D.h"
#include "ShadowMapCube.h"

LightSystem::LightSystem(float zNear, float zFar)
    : shadowSystem(std::make_unique<ShadowSystem>()), zNear(zNear), zFar(zFar), icoSphere("Assets/Models/icoSphere.obj")
{
}
void LightSystem::DrawLightSpheres(unsigned int shaderID, const std::vector<Light>& lights) const
{
    glEnable(GL_CULL_FACE);
    for (const auto& light : lights)
    {
        glm::vec4 col = glm::vec4(light.getColor(), 1.0f);
        glUniform4fv(ShaderManager::getLoc(shaderID, "lightColor"), 1, glm::value_ptr(col));
        icoSphere.Draw(shaderID, light.getPosition(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    }
    glDisable(GL_CULL_FACE);
}

void LightSystem::ExportUniforms(unsigned int shaderID, const std::vector<Light>& lights) const
{
    shadowSystem->BindShadowTextures(shaderID, 3 /*start slot*/);

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

unsigned int LightSystem::getShaderIDfromType(LightType type) const
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

void LightSystem::ShadowPass(const std::vector<Model>& models, const std::vector<Light>& lights) const
{
    shadowSystem->ClearAllTargets();

    for (const auto& light : lights)
    {
        const unsigned int shaderID = getShaderIDfromType(light.getType());
        light.caster->BeginDepthPass(shaderID, *shadowSystem, light.getPosition());
        for (const auto& model : models)
            model.Draw(shaderID);
        light.caster->EndDepthPass();
    }
}

void LightSystem::addLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top)
{
    auto caster = std::make_unique<ShadowMap2D>(
        shadowSystem->RegisterCaster(LightType::DIRECTION),
        lightPos, direction, left, right, bottom, top, zNear, zFar);
    lights.emplace_back(LightType::DIRECTION, lightPos, direction, lightColor, std::move(caster));
}

void LightSystem::addLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone)
{
    auto caster = std::make_unique<ShadowMap2D>(
        shadowSystem->RegisterCaster(LightType::SPOT),
        lightPos, direction, fovDeg, innerCone, outerCone, zNear, zFar);
    lights.emplace_back(LightType::SPOT, lightPos, direction, lightColor, std::move(caster));
}

void LightSystem::addLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 lightColor)
{
    auto caster = std::make_unique<ShadowMapCube>(
        shadowSystem->RegisterCaster(LightType::POINT),
        lightPos, zNear, zFar);
    lights.emplace_back(LightType::POINT, lightPos, glm::vec3(0.0f), lightColor, std::move(caster));
}