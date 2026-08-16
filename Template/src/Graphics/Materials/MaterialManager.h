#ifndef MATERIAL_MANAGER_NAMESPACE_H
#define MATERIAL_MANAGER_NAMESPACE_H

#include <deque>
#include <unordered_set>

#include "Material.h"

namespace MaterialManager
{
extern std::deque<Material>                                      materials;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedDiffuseTextures;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedSpecularTextures;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedNormalTextures;
extern std::unordered_map<std::string, std::shared_ptr<Texture>> loadedDisplacementTextures;

unsigned int LoadMaterial(const std::string& name,
                          const std::string& diffuseMapPath,
                          const std::string& specularMapPath,
                          const std::string& normalMapPath,
                          const std::string& displacementMapPath);

unsigned int LoadMaterial(const std::string& name,
                          glm::vec3          diffuseColor,
                          glm::vec3          specularColor,
                          float              shininess,
                          const std::string& diffuseMapPath,
                          const std::string& specularMapPath,
                          const std::string& normalMapPath,
                          const std::string& displacementMapPath);

std::shared_ptr<Texture> makeTexture(const std::string& texturePath, const std::string& type);

Material& getMatAt(int ID);
void      Unbind();

};  // namespace MaterialManager
#endif