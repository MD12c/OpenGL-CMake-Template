#include "PBRMaterial.h"

#include "glm/gtc/type_ptr.hpp"

#include "Globals.h"

PBRMaterial::PBRMaterial(int                      ID,
                         std::string              name,
                         float                    roughness,
                         float                    metalic,
                         std::shared_ptr<Texture> albedoMap,
                         std::shared_ptr<Texture> aoMap,
                         std::shared_ptr<Texture> metalicRoughnessMap,
                         std::shared_ptr<Texture> normalMap,
                         std::shared_ptr<Texture> displacementMap)

    : Material(ID),
      name(name),
      roughness(roughness),
      metalic(metalic),
      albedoMap(albedoMap),
      aoMap(aoMap),
      metalicRoughnessMap(metalicRoughnessMap),
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
    glUniform1i(ShaderManager::getLoc(shaderID, "useAO"), (aoMap != nullptr));
    glUniform1i(ShaderManager::getLoc(shaderID, "useRoughness"), (metalicRoughnessMap != nullptr));
    // glUniform1i(ShaderManager::getLoc(shaderID, "useRoughness"), 0);
    glUniform1i(ShaderManager::getLoc(shaderID, "useMetalic"), (metalicRoughnessMap != nullptr));
    // glUniform1i(ShaderManager::getLoc(shaderID, "useMetalic"), 0);
    glUniform1i(ShaderManager::getLoc(shaderID, "useNormal"), (normalMap != nullptr));
    glUniform1i(ShaderManager::getLoc(shaderID, "useDisplacement"), (displacementMap != nullptr));

    auto checkAndLoad = [&](std::shared_ptr<Texture> texPtr, const std::string& name)
    {
        if (texPtr)
        {
            texPtr->Bind();
            texPtr->texUnit(shaderID, name);
        }
    };
    checkAndLoad(albedoMap, "albedo0");
    checkAndLoad(aoMap, "ao0");
    checkAndLoad(metalicRoughnessMap, "metalicRoughness0");
    checkAndLoad(normalMap, "normal0");
    checkAndLoad(displacementMap, "displacement0");
}
