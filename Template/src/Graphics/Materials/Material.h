#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include "glm/glm.hpp"
#include "../GLObjects/Texture.h"

class Material
{
public:
    int         ID;
    std::string name;  // optional
    glm::vec3   diffuseColor  = glm::vec3(1.0f);
    glm::vec3   specularColor = glm::vec3(1.0f);
    float       shininess     = 16.0f;

    std::shared_ptr<Texture> diffuseMap  = nullptr;
    std::shared_ptr<Texture> specularMap = nullptr;
    std::shared_ptr<Texture> normalMap   = nullptr;

    Material(int                      ID,
             std::string              name,
             glm::vec3                diffuseColor,
             glm::vec3                specularColor,
             float                    shininess,
             std::shared_ptr<Texture> diffuseMap,
             std::shared_ptr<Texture> specularMap,
             std::shared_ptr<Texture> normalMap);

    Material(int                      ID,
             std::string              name,
             std::shared_ptr<Texture> diffuseMap,
             std::shared_ptr<Texture> specularMap,
             std::shared_ptr<Texture> normalMap);

    void Apply(unsigned int shaderID) const;

    void Bind() const;
};

#endif