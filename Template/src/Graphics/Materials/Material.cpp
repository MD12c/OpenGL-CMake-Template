#include "Material.h"

#include "glm/gtc/type_ptr.hpp"

Material::Material(int                      ID,
                   std::string              name,
                   glm::vec3                diffuseColor,
                   glm::vec3                specularColor,
                   float                    shininess,
                   std::shared_ptr<Texture> diffuseMap,
                   std::shared_ptr<Texture> specularMap,
                   std::shared_ptr<Texture> normalMap,
                   std::shared_ptr<Texture> displacementMap)

    : ID(ID),
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

Material::Material(int                      ID,
                   std::string              name,
                   std::shared_ptr<Texture> diffuseMap,
                   std::shared_ptr<Texture> specularMap,
                   std::shared_ptr<Texture> normalMap,
                   std::shared_ptr<Texture> displacementMap)

    : ID(ID),
      name(name),
      diffuseMap(diffuseMap),
      specularMap(specularMap),
      normalMap(normalMap),
      displacementMap(displacementMap)
{
}

void Material::Apply(int shaderID) const
{
    ShaderManager::Activate(shaderID);
    glUniform3fv(ShaderManager::getLoc(shaderID, "diffuseColor"), 1, glm::value_ptr(diffuseColor));
    glUniform1f(ShaderManager::getLoc(shaderID, "shininess"), shininess);

    glUniform1i(ShaderManager::getLoc(shaderID, "useTexture"), (diffuseMap != nullptr));
    glUniform1i(ShaderManager::getLoc(shaderID, "useNormal"), (normalMap != nullptr));
    // glUniform1i(ShaderManager::getLoc(shaderID, "useNormal"), 0);
    glUniform1i(ShaderManager::getLoc(shaderID, "useDisplacement"), (displacementMap != nullptr));
    // glUniform1i(ShaderManager::getLoc(shaderID, "useDisplacement"), 0);

    Bind();
    if (diffuseMap)
        diffuseMap->texUnit(shaderID, "diffuse0");
    if (specularMap)
        specularMap->texUnit(shaderID, "specular0");
    if (normalMap)
        normalMap->texUnit(shaderID, "normal0");
    if (displacementMap)
        displacementMap->texUnit(shaderID, "displacement0");
}

void Material::Bind() const
{
    if (diffuseMap)
        diffuseMap->Bind();
    if (specularMap)
        specularMap->Bind();
    if (normalMap)
        normalMap->Bind();
    if (displacementMap)
        displacementMap->Bind();
}