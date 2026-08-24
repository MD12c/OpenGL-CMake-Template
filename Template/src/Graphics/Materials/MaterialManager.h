#ifndef MATERIAL_MANAGER_NAMESPACE_H
#define MATERIAL_MANAGER_NAMESPACE_H

#include <deque>
#include <unordered_map>

#include "SpecularMaterial.h"
#include "PBRMaterial.h"
#include "Material.h"

namespace MaterialManager
{
extern std::deque<std::unique_ptr<Material>>                                                               materials;
extern std::unordered_map<Texture::TextureType, std::unordered_map<std::string, std::shared_ptr<Texture>>> loadedTextures;

int LoadMaterialSpecular(const std::string& name,
                         glm::vec3          diffuseColor,
                         glm::vec3          specularColor,
                         float              shininess,
                         const std::string& diffuseMapPath,
                         const std::string& specularMapPath,
                         const std::string& normalMapPath,
                         const std::string& displacementMapPath);

int LoadMaterialPBRobj(const std::string& name,
                       float              roughness,
                       float              metalic,
                       const std::string& albedoMapPath,
                       const std::string& aoMapPath,
                       const std::string& roughnessMapPath,
                       const std::string& metalicMapPath,
                       const std::string& normalMapPath,
                       const std::string& displacementMapPath);

int LoadMaterialPBRgltf(const std::string& name,
                        float              roughness,
                        float              metalic,
                        const std::string& albedoMapPath,
                        const std::string& aoMapPath,
                        const std::string& metalicRoughnessMapPath,
                        const std::string& normalMapPath,
                        const std::string& displacementMapPath);

std::shared_ptr<Texture> makeTexture(const std::string& texturePath, Texture::TextureType type);
std::shared_ptr<Texture> makeTexture(const std::string& texturePath1, const std::string& texturePath2, Texture::TextureType type);

Material& getMatAt(int ID);
void      Unbind(int ID);

};  // namespace MaterialManager
#endif