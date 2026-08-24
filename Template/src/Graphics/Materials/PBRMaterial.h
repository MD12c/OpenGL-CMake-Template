#ifndef PBR_MATERIAL_CLASS_H
#define PBR_MATERIAL_CLASS_H

#include "glm/glm.hpp"
#include "..\GLObjects\Texture.h"
#include "Material.h"
#include "../Shaders/ShaderManager.h"

class PBRMaterial : public Material
{
public:
    static constexpr ShaderIDs shaderID = ShaderIDs::PBR;

    std::string name;  // optional
    glm::vec3   albedoColor = glm::vec3(0.7f);
    float       roughness   = 0.04f;
    float       metalic     = 0.0f;

    std::shared_ptr<Texture> albedoMap           = nullptr;
    std::shared_ptr<Texture> aoMap               = nullptr;
    std::shared_ptr<Texture> metalicRoughnessMap = nullptr;
    std::shared_ptr<Texture> normalMap           = nullptr;
    std::shared_ptr<Texture> displacementMap     = nullptr;

    PBRMaterial(int                      ID,
                std::string              name,
                float                    roughness,
                float                    metalic,
                std::shared_ptr<Texture> albedoMap,
                std::shared_ptr<Texture> aoMap,
                std::shared_ptr<Texture> metalicRoughnessMap,
                std::shared_ptr<Texture> normalMap,
                std::shared_ptr<Texture> displacementMap);

    void      Apply() const override;
    ShaderIDs getShaderID() const override { return shaderID; };
};

#endif