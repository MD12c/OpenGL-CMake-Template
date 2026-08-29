#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Shaders/Shader.h"
#include "../Materials/MaterialManager.h"
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

    void Draw(ShaderID shaderID, Transform transform = {}, const Frustum* frustum = nullptr, MaterialID materialID = USE_FILE_MATERIAL) const;

    void setMeshMetalicRoughness(int meshIndex, float metalic, float roughness);
    void setCustomMaterial(MaterialID materialID);
};
