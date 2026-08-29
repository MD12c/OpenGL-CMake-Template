#ifndef SPECULAR_MATERIAL_CLASS_H
#define SPECULAR_MATERIAL_CLASS_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "..\GLObjects\Texture.h"
#include "Material.h"
#include "../Shaders/Shader.h"

class SpecularMaterial : public Material
{
public:
    static constexpr ShaderID shaderID = ShaderID::SPECULAR;

    std::string name;  // optional
    glm::vec3   diffuseColor  = glm::vec3(1.0f);
    glm::vec3   specularColor = glm::vec3(1.0f);
    float       shininess     = 16.0f;

    std::shared_ptr<Texture> diffuseMap      = nullptr;
    std::shared_ptr<Texture> specularMap     = nullptr;
    std::shared_ptr<Texture> normalMap       = nullptr;
    std::shared_ptr<Texture> displacementMap = nullptr;

    SpecularMaterial(int                      ID,
                     std::string              name,
                     glm::vec3                diffuseColor,
                     glm::vec3                specularColor,
                     float                    shininess,
                     std::shared_ptr<Texture> diffuseMap,
                     std::shared_ptr<Texture> specularMap,
                     std::shared_ptr<Texture> normalMap,
                     std::shared_ptr<Texture> displacementMap)
        : Material(ID, ShaderID::SPECULAR),
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

    void Apply() const override
    {
        Shader::Activate(shaderID);
        glUniform3fv(Shader::getLoc(shaderID, "diffuseColor"), 1, glm::value_ptr(diffuseColor));
        glUniform1f(Shader::getLoc(shaderID, "shininess"), shininess);

        glUniform1i(Shader::getLoc(shaderID, "useTexture"), (diffuseMap != nullptr));
        glUniform1i(Shader::getLoc(shaderID, "useNormal"), (normalMap != nullptr));
        glUniform1i(Shader::getLoc(shaderID, "useDisplacement"), (displacementMap != nullptr));

        checkAndLoad(diffuseMap, "diffuse0", shaderID);
        checkAndLoad(specularMap, "specular0", shaderID);
        checkAndLoad(normalMap, "normal0", shaderID);
        checkAndLoad(displacementMap, "displacement0", shaderID);
    }
};

#endif