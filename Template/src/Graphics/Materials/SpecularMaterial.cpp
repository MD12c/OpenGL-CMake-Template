#include "SpecularMaterial.h"

#include "glm/gtc/type_ptr.hpp"

SpecularMaterial::SpecularMaterial(int                      ID,
                                   std::string              name,
                                   glm::vec3                diffuseColor,
                                   glm::vec3                specularColor,
                                   float                    shininess,
                                   std::shared_ptr<Texture> diffuseMap,
                                   std::shared_ptr<Texture> specularMap,
                                   std::shared_ptr<Texture> normalMap,
                                   std::shared_ptr<Texture> displacementMap)
    : Material(ID),
      name(name),
      diffuseColor(diffuseColor),
      specularColor(specularColor),
      shininess(shininess),
      diffuseMap(diffuseMap),
      specularMap(specularMap),
      normalMap(normalMap),
      displacementMap(displacementMap)
{
}

void SpecularMaterial::Apply() const
{
    ShaderManager::Activate(shaderID);
    glUniform3fv(ShaderManager::getLoc(shaderID, "diffuseColor"), 1, glm::value_ptr(diffuseColor));
    glUniform1f(ShaderManager::getLoc(shaderID, "shininess"), shininess);

    glUniform1i(ShaderManager::getLoc(shaderID, "useTexture"), (diffuseMap != nullptr));
    glUniform1i(ShaderManager::getLoc(shaderID, "useNormal"), (normalMap != nullptr));
    glUniform1i(ShaderManager::getLoc(shaderID, "useDisplacement"), (displacementMap != nullptr));

    auto checkAndLoad = [&](std::shared_ptr<Texture> texPtr, const std::string& name)
    {
        if (!texPtr) return;
        texPtr->texUnit(shaderID, name);
    };

    checkAndLoad(diffuseMap, "diffuse0");
    checkAndLoad(specularMap, "specular0");
    checkAndLoad(normalMap, "normal0");
    checkAndLoad(displacementMap, "displacement0");
}
