#include "MaterialManager.h"

using enum Texture::TextureType;

namespace MaterialManager
{
std::deque<std::unique_ptr<Material>>                                                               materials      = {};
std::unordered_map<Texture::TextureType, std::unordered_map<std::string, std::shared_ptr<Texture>>> loadedTextures = {};

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

int LoadMaterialPBRobj(const std::string& name,
                       float              roughness,
                       float              metalic,
                       const std::string& albedoMapPath,
                       const std::string& aoMapPath,
                       const std::string& roughnessMapPath,
                       const std::string& metalicMapPath,
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
            makeTexture(roughnessMapPath, metalicMapPath, METALIC_ROUGHNESS),
            makeTexture(normalMapPath, NORMAL),
            makeTexture(displacementMapPath, DISPLACEMENT)));

    return materials.back()->ID;
}

int LoadMaterialPBRgltf(const std::string& name,
                        float              roughness,
                        float              metalic,
                        const std::string& albedoMapPath,
                        const std::string& aoMapPath,
                        const std::string& metalicRoughnessMapPath,
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
            makeTexture(metalicRoughnessMapPath, METALIC_ROUGHNESS),
            makeTexture(normalMapPath, NORMAL),
            makeTexture(displacementMapPath, DISPLACEMENT)));

    return materials.back()->ID;
}

int assignUnit(Texture::TextureType type)
{
    switch (type)
    {
        case DIFFUSE:
            return 0;
        case ALBEDO:
            return 0;
        case SPECULAR:
            return 1;
        case AO:
            return 1;
        case METALIC_ROUGHNESS:
            return 2;
        case NORMAL:
            return 4;
        case DISPLACEMENT:
            return 5;
        default:
            throw std::runtime_error("[ERROR] invalid texture type");
    }
}

std::shared_ptr<Texture> makeTexture(const std::string& texturePath, Texture::TextureType type)
{
    if (texturePath.empty())
        return nullptr;

    auto& cache = loadedTextures[type];

    auto it = cache.find(texturePath);
    if (it != cache.end())
        return it->second;

    auto newTexture    = std::make_shared<Texture>(texturePath, type, assignUnit(type));
    cache[texturePath] = newTexture;
    return newTexture;
}

std::shared_ptr<Texture> makeTexture(const std::string& texturePath1, const std::string& texturePath2, Texture::TextureType type)
{
    if (texturePath1.empty() && texturePath2.empty())
        return nullptr;
    else if (texturePath1.empty())
        return makeTexture(texturePath2, METALIC_ROUGHNESS);
    else if (texturePath2.empty())
        return makeTexture(texturePath1, METALIC_ROUGHNESS);

    std::string createdTexture = texturePath1 + texturePath2;

    auto& cache = loadedTextures[type];

    auto it = cache.find(createdTexture);
    if (it != cache.end())
        return it->second;

    auto newTexture       = std::make_shared<Texture>(texturePath1, texturePath2, type, assignUnit(type), Texture::TextureCombineMode::Pack);
    cache[createdTexture] = newTexture;
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