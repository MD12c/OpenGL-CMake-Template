#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Frustum/Frustum.h"
#include "Transform.h"

class Model
{
private:
    std::vector<Mesh> meshes;

    void loadModel(const std::string& path);
    void processNode(aiNode*, const aiScene*);
    Mesh processMesh(aiMesh*, const aiScene*);

public:
    std::string directory;
    std::string fileType;

    Model(const std::string& path);
    Model(std::vector<Mesh> meshes) : meshes(std::move(meshes)) {}

    Model(const Model&)                = delete;
    Model& operator=(const Model&)     = delete;
    Model(Model&&) noexcept            = default;
    Model& operator=(Model&&) noexcept = default;

    void Draw(ShaderID shaderID, Transform transform = {}, const Frustum* frustum = nullptr, MaterialID materialID = USE_FILE_MATERIAL) const;
    void DrawSimple(ShaderID shaderID) const;

    void setMeshMetalicRoughness(int meshIndex, float metalic, float roughness);
    void setCustomMaterial(MaterialID materialID);
};
