#include "MaterialManager.h"

namespace MaterialManager
{
std::deque<Material>                                      materials              = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedDiffuseTextures  = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedSpecularTextures = {};
std::unordered_map<std::string, std::shared_ptr<Texture>> loadedNormalTextures   = {};

unsigned int LoadMaterial(const std::string& name,
                          const std::string& diffuseMapPath,
                          const std::string& specularMapPath,
                          const std::string& normalMapPath)
{
    materials.emplace_back(
        Material(
            (int)materials.size(),
            name,
            makeTexture(diffuseMapPath, "diffuse"),
            makeTexture(specularMapPath, "specular"),
            makeTexture(normalMapPath, "normal")));

    return materials.back().ID;
}

unsigned int LoadMaterial(const std::string& name,
                          glm::vec3          diffuseColor,
                          glm::vec3          specularColor,
                          float              shininess,
                          const std::string& diffuseMapPath,
                          const std::string& specularMapPath,
                          const std::string& normalMapPath)
{
    materials.emplace_back(
        Material(
            (int)materials.size(),
            name,
            diffuseColor,
            specularColor,
            shininess,
            makeTexture(diffuseMapPath, "diffuse"),
            makeTexture(specularMapPath, "specular"),
            makeTexture(normalMapPath, "normal")));

    return materials.back().ID;
}

std::shared_ptr<Texture> makeTexture(const std::string& texturePath, const std::string& type)
{
    if (texturePath.empty())
        return nullptr;

    std::unordered_map<std::string, std::shared_ptr<Texture>>* cache;
    int                                                        unit;
    if (type == "diffuse")
    {
        unit  = 0;
        cache = &loadedDiffuseTextures;
    }
    else if (type == "specular")
    {
        unit  = 1;
        cache = &loadedSpecularTextures;
    }
    else if (type == "normal")
    {
        unit  = 2;
        cache = &loadedNormalTextures;
    }
    else
        throw std::runtime_error("[ERROR] invalid texture type");

    auto it = cache->find(texturePath);
    if (it != cache->end())
        return it->second;

    auto newTexture       = std::make_shared<Texture>(texturePath, type, unit);
    (*cache)[texturePath] = newTexture;
    return newTexture;
}

Material& getMatAt(int ID)
{
    return materials.at(ID);
}

void Unbind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
}

};  // namespace MaterialManager