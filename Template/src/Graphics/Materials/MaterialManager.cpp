#include "MaterialManager.h"

#include "Globals.h"

namespace MaterialManager
{
std::deque<std::unique_ptr<Material>>                                                               materials      = {};
std::unordered_map<Texture::TextureType, std::unordered_map<std::string, std::shared_ptr<Texture>>> loadedTextures = {};

template <typename MaterialT, typename... Args>
MaterialID AddMaterial(Args&&... args)
{
    materials.emplace_back(std::make_unique<MaterialT>(static_cast<MaterialID>(materials.size()), std::forward<Args>(args)...));
    return materials.back()->ID;
}

MaterialID LoadMaterialSpecular(
    const std::string& name,
    glm::vec3          diffuseColor,
    glm::vec3          specularColor,
    float              shininess,
    const std::string& diffuseMapPath,
    const std::string& specularMapPath,
    const std::string& normalMapPath,
    const std::string& displacementMapPath)
{
    return AddMaterial<SpecularMaterial>(
        name, diffuseColor, specularColor, shininess,
        makeTexture(diffuseMapPath, Texture::TextureType::DIFFUSE),
        makeTexture(specularMapPath, Texture::TextureType::SPECULAR),
        makeTexture(normalMapPath, Texture::TextureType::NORMAL),
        makeTexture(displacementMapPath, Texture::TextureType::DISPLACEMENT));
}

MaterialID LoadMaterialPBRobj(
    const std::string& name,
    float              roughness,
    float              metalic,
    const std::string& albedoMapPath,
    const std::string& aoMapPath,
    const std::string& roughnessMapPath,
    const std::string& metalicMapPath,
    const std::string& normalMapPath,
    const std::string& displacementMapPath)
{
    return AddMaterial<PBRMaterial>(
        name, roughness, metalic,
        makeTexture(albedoMapPath, Texture::TextureType::ALBEDO),
        makeTexture(aoMapPath, Texture::TextureType::AO),
        makeTexture(roughnessMapPath, metalicMapPath, Texture::TextureType::METALIC_ROUGHNESS),
        makeTexture(normalMapPath, Texture::TextureType::NORMAL),
        makeTexture(displacementMapPath, Texture::TextureType::DISPLACEMENT));
}

MaterialID LoadMaterialPBRgltf(
    const std::string& name,
    float              roughness,
    float              metalic,
    const std::string& albedoMapPath,
    const std::string& aoMapPath,
    const std::string& metalicRoughnessMapPath,
    const std::string& normalMapPath,
    const std::string& displacementMapPath)
{
    return AddMaterial<PBRMaterial>(
        name, roughness, metalic,
        makeTexture(albedoMapPath, Texture::TextureType::ALBEDO),
        makeTexture(aoMapPath, Texture::TextureType::AO),
        makeTexture(metalicRoughnessMapPath, Texture::TextureType::METALIC_ROUGHNESS),
        makeTexture(normalMapPath, Texture::TextureType::NORMAL),
        makeTexture(displacementMapPath, Texture::TextureType::DISPLACEMENT));
}

MaterialID LoadMaterialCustom(const std::string& name, ShaderID shaderID)
{
    materials.emplace_back(std::make_unique<Material>((int)materials.size(), shaderID));
    return materials.back()->ID;
}

std::shared_ptr<Texture> makeTexture(const std::string& texturePath, Texture::TextureType type)
{
    if (texturePath.empty())
        return nullptr;

    auto& cache = loadedTextures[type];

    auto it = cache.find(texturePath);
    if (it != cache.end())
        return it->second;

    auto newTexture    = std::make_shared<Texture>(texturePath, type);
    cache[texturePath] = newTexture;
    return newTexture;
}

std::shared_ptr<Texture> makeTexture(const std::string& texturePath1, const std::string& texturePath2, Texture::TextureType type)
{
    if (texturePath1.empty() && texturePath2.empty())
        return nullptr;
    else if (texturePath1.empty())
        return makeTexture(texturePath2, Texture::TextureType::METALIC_ROUGHNESS);
    else if (texturePath2.empty())
        return makeTexture(texturePath1, Texture::TextureType::METALIC_ROUGHNESS);

    std::string createdTexture = texturePath1 + texturePath2;

    auto& cache = loadedTextures[type];

    auto it = cache.find(createdTexture);
    if (it != cache.end())
        return it->second;

    auto newTexture       = std::make_shared<Texture>(texturePath1, texturePath2, type, Texture::TextureCombineMode::Pack);
    cache[createdTexture] = newTexture;
    return newTexture;
}

};  // namespace MaterialManager