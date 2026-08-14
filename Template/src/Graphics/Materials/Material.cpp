#include "Material.h"

#include "glm/gtc/type_ptr.hpp"

Material::Material(int                      ID,
                   std::string              name,
                   glm::vec3                diffuseColor,
                   glm::vec3                specularColor,
                   float                    shininess,
                   std::shared_ptr<Texture> diffuseMap,
                   std::shared_ptr<Texture> specularMap)

    : ID(ID),
      name(name),
      diffuseColor(diffuseColor),
      specularColor(specularColor),
      shininess(shininess),
      diffuseMap(diffuseMap),
      specularMap(specularMap)
{
}

Material::Material(int                      ID,
                   std::string              name,
                   std::shared_ptr<Texture> diffuseMap,
                   std::shared_ptr<Texture> specularMap)

    : ID(ID),
      name(name),
      diffuseMap(diffuseMap),
      specularMap(specularMap)
{
}

void Material::Apply(unsigned int shaderID) const
{
    ShaderManager::Activate(shaderID);
    glUniform3fv(ShaderManager::getLoc(shaderID, "diffuseColor"), 1, glm::value_ptr(diffuseColor));
    glUniform1f(ShaderManager::getLoc(shaderID, "shininess"), shininess);

    bool hasTexture = (diffuseMap != nullptr);
    glUniform1i(ShaderManager::getLoc(shaderID, "useTexture"), hasTexture);

    Bind();
    if (diffuseMap)
        diffuseMap->texUnit(shaderID, "diffuse0");
    if (specularMap)
        specularMap->texUnit(shaderID, "specular0");
}

void Material::Bind() const
{
    if (diffuseMap)
        diffuseMap->Bind();
    if (specularMap)
        specularMap->Bind();
}