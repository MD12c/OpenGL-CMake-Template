#ifndef PBR_MATERIAL_CLASS_H
#define PBR_MATERIAL_CLASS_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "..\GLObjects\Texture.h"
#include "Material.h"
#include "../Shaders/Shader.h"

class PBRMaterial : public Material
{
public:
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
                std::shared_ptr<Texture> displacementMap)

        : Material(ID, ShaderID::PBR),
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

    void Apply() const override
    {
        Shader::Activate(shaderID);
        glUniform3fv(Shader::getLoc(shaderID, "albedoColor"), 1, glm::value_ptr(albedoColor));
        glUniform1f(Shader::getLoc(shaderID, "roughness"), roughness);
        glUniform1f(Shader::getLoc(shaderID, "metalic"), metalic);

        glUniform1i(Shader::getLoc(shaderID, "useTexture"), (albedoMap != nullptr));
        glUniform1i(Shader::getLoc(shaderID, "useAO"), (aoMap != nullptr));
        glUniform1i(Shader::getLoc(shaderID, "useRoughness"), (metalicRoughnessMap != nullptr));
        // glUniform1i(Shader::getLoc(shaderID, "useRoughness"), 0);
        glUniform1i(Shader::getLoc(shaderID, "useMetalic"), (metalicRoughnessMap != nullptr));
        // glUniform1i(Shader::getLoc(shaderID, "useMetalic"), 0);
        glUniform1i(Shader::getLoc(shaderID, "useNormal"), (normalMap != nullptr));
        glUniform1i(Shader::getLoc(shaderID, "useDisplacement"), (displacementMap != nullptr));

        checkAndLoad(albedoMap, "albedo0", shaderID);
        checkAndLoad(aoMap, "ao0", shaderID);
        checkAndLoad(metalicRoughnessMap, "metalicRoughness0", shaderID);
        checkAndLoad(normalMap, "normal0", shaderID);
        checkAndLoad(displacementMap, "displacement0", shaderID);
    }
};

#endif