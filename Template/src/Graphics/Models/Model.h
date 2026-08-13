#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Shaders/ShaderManager.h"

class Model
{
private:
    std::vector<Mesh> meshes;

    void                 loadModel(const std::string& path);
    void                 processNode(aiNode*, const aiScene*);
    Mesh                 processMesh(aiMesh*, const aiScene*);
    std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string);

public:
    std::string          directory;
    std::vector<Texture> loadedTextures;

    Model(const std::string& path);

    void Draw(unsigned int shaderID) const;
    void Draw(unsigned int shaderID, glm::mat4 model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale) const;
};
