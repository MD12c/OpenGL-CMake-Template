#ifndef MATERIAL_MANAGER_NAMESPACE_H
#define MATERIAL_MANAGER_NAMESPACE_H

#include <deque>
#include <unordered_map>

#include "SpecularMaterial.h"
#include "PBRMaterial.h"
#include "Material.h"

namespace MaterialManager
{
extern std::deque<std::unique_ptr<Material>>                     materials;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedDiffuseTextures;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedAlbedoTextures;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedAOTextures;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedSpecularTextures;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedNormalTextures;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedDisplacementTextures;

int LoadMaterialSpecular(const std::string& name,
                         const std::string& diffuseMapPath,
                         const std::string& specularMapPath,
                         const std::string& normalMapPath,
                         const std::string& displacementMapPath);

int LoadMaterialSpecular(const std::string& name,
                         glm::vec3          diffuseColor,
                         glm::vec3          specularColor,
                         float              shininess,
                         const std::string& diffuseMapPath,
                         const std::string& specularMapPath,
                         const std::string& normalMapPath,
                         const std::string& displacementMapPath);

int LoadMaterialPBR(const std::string& name,
                    float              roughness,
                    float              metalic,
                    const std::string& albedoMapPath,
                    const std::string& aoMapPath,
                    const std::string& normalMapPath,
                    const std::string& displacementMapPath);


std::shared_ptr<Texture> makeTexture(const std::string& texturePath, Texture::TextureType type);

Material& getMatAt(int ID);
void      Unbind();

};  // namespace MaterialManager
#endif