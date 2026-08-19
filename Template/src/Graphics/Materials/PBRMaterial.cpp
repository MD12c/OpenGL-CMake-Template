#include "PBRMaterial.h"

#include "glm/gtc/type_ptr.hpp"

PBRMaterial::PBRMaterial(int                      ID,
                         std::string              name,
                         float                    roughness,
                         float                    metalic,
                         std::shared_ptr<Texture> albedoMap,
                         std::shared_ptr<Texture> aoMap,
                         std::shared_ptr<Texture> normalMap,
                         std::shared_ptr<Texture> displacementMap)

    : Material(ID),
      name(name),
      roughness(roughness),
      metalic(metalic),
      albedoMap(albedoMap),
      aoMap(aoMap),
      normalMap(normalMap),
      displacementMap(displacementMap)
{
}

PBRMaterial::PBRMaterial(int                      ID,
                         std::string              name,
                         std::shared_ptr<Texture> albedoMap,
                         std::shared_ptr<Texture> aoMap,
                         std::shared_ptr<Texture> normalMap,
                         std::shared_ptr<Texture> displacementMap)
    : Material(ID),
      name(name),
      albedoMap(albedoMap),
      aoMap(aoMap),
      normalMap(normalMap),
      displacementMap(displacementMap)
{
}

void PBRMaterial::Apply(int shaderID) const
{
    ShaderManager::Activate(shaderID);
    glUniform3fv(ShaderManager::getLoc(shaderID, "albedoColor"), 1, glm::value_ptr(albedoColor));
    glUniform1f(ShaderManager::getLoc(shaderID, "roughness"), roughness);
    glUniform1f(ShaderManager::getLoc(shaderID, "metalic"), metalic);

    glUniform1i(ShaderManager::getLoc(shaderID, "useTexture"), (albedoMap != nullptr));
    glUniform1i(ShaderManager::getLoc(shaderID, "useNormal"), (normalMap != nullptr));
    glUniform1i(ShaderManager::getLoc(shaderID, "useDisplacement"), (displacementMap != nullptr));

    if (albedoMap)
    {
        albedoMap->Bind();
        albedoMap->texUnit(shaderID, "albedo0");
    }
    if (aoMap)
    {
        aoMap->Bind();
        aoMap->texUnit(shaderID, "ao0");
    }
    if (normalMap)
    {
        normalMap->Bind();
        normalMap->texUnit(shaderID, "normal0");
    }
    if (displacementMap)
    {
        displacementMap->Bind();
        displacementMap->texUnit(shaderID, "displacement0");
    }
}
