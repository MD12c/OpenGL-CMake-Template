#include "MaterialManager.h"

using enum Texture::TextureType;

namespace MaterialManager
{
std::deque<std::unique_ptr<Material>>                     materials                  = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedDiffuseTextures      = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedAlbedoTextures       = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedSpecularTextures     = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedAOTextures           = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedNormalTextures       = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedDisplacementTextures = {};

int LoadMaterialSpecular(const std::string& name,
                         const std::string& diffuseMapPath,
                         const std::string& specularMapPath,
                         const std::string& normalMapPath,
                         const std::string& displacementMapPath)
{
    materials.emplace_back(
        std::make_unique<SpecularMaterial>(
            (int)materials.size(),
            name,
            makeTexture(diffuseMapPath, DIFFUSE),
            makeTexture(specularMapPath, SPECULAR),
            makeTexture(normalMapPath, NORMAL),
            makeTexture(displacementMapPath, DISPLACEMENT)));

    return materials.back()->ID;
}

int LoadMaterialSpecular(const std::string& name,
                         glm::vec3          diffuseColor,
                         glm::vec3          specularColor,
                         float              shininess,
                         const std::string& diffuseMapPath,
                         const std::string& specularMapPath,
                         const std::string& normalMapPath,
                         const std::string& displacementMapPath)
{
    materials.emplace_back(
        std::make_unique<SpecularMaterial>(
            (int)materials.size(),
            name,
            diffuseColor,
            specularColor,
            shininess,
            makeTexture(diffuseMapPath, DIFFUSE),
            makeTexture(specularMapPath, SPECULAR),
            makeTexture(normalMapPath, NORMAL),
            makeTexture(displacementMapPath, DISPLACEMENT)));

    return materials.back()->ID;
}

int LoadMaterialPBR(const std::string& name,
                    float              roughness,
                    float              metalic,
                    const std::string& albedoMapPath,
                    const std::string& aoMapPath,
                    const std::string& normalMapPath,
                    const std::string& displacementMapPath)
{
    materials.emplace_back(
        std::make_unique<PBRMaterial>(
            (int)materials.size(),
            name,
            roughness,
            metalic,
            makeTexture(albedoMapPath, ALBEDO),
            makeTexture(aoMapPath, AO),
            makeTexture(normalMapPath, NORMAL),
            makeTexture(displacementMapPath, DISPLACEMENT)));

    return materials.back()->ID;
}

std::shared_ptr<Texture> makeTexture(const std::string& texturePath, Texture::TextureType type)
{
    if (texturePath.empty())
        return nullptr;

    std::unordered_map<std::string, std::shared_ptr<Texture>>* cache;
    int                                                        unit;

    switch (type)
    {
        case DIFFUSE:
            unit  = 0;
            cache = &loadedDiffuseTextures;
            break;
        case ALBEDO:
            unit  = 0;
            cache = &loadedAlbedoTextures;
            break;
        case SPECULAR:
            unit  = 1;
            cache = &loadedSpecularTextures;
            break;
        case AO:
            unit  = 1;
            cache = &loadedAOTextures;
            break;
        case NORMAL:
            unit  = 2;
            cache = &loadedNormalTextures;
            break;
        case DISPLACEMENT:
            unit  = 3;
            cache = &loadedDisplacementTextures;
            break;
        default:
            throw std::runtime_error("[ERROR] invalid texture type");
    }

    auto it = cache->find(texturePath);
    if (it != cache->end())
        return it->second;

    auto newTexture       = std::make_shared<Texture>(texturePath, type, unit);
    (*cache)[texturePath] = newTexture;
    return newTexture;
}

Material& getMatAt(int ID)
{
    return *materials.at(ID);
}

void Unbind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0);
}

};  // namespace MaterialManager