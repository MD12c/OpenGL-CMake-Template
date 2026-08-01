#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

    void Draw(Shader&);
};
