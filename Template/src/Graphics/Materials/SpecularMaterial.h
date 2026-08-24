#ifndef SPECULAR_MATERIAL_CLASS_H
#define SPECULAR_MATERIAL_CLASS_H

#include "glm/glm.hpp"
#include "..\GLObjects\Texture.h"
#include "Material.h"
#include "../Shaders/ShaderManager.h"

class SpecularMaterial : public Material
{
public:
    static constexpr ShaderIDs shaderID = ShaderIDs::SPECULAR;

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
                     std::shared_ptr<Texture> displacementMap);

    void Apply() const override;
    ShaderIDs getShaderID() const override { return shaderID; };
};

#endif