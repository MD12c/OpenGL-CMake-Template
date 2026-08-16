#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Shaders/ShaderManager.h"
#include "../Materials/MaterialManager.h"

class Model
{
private:
    std::vector<Mesh> meshes;

    void loadModel(const std::string& path);
    void processNode(aiNode*, const aiScene*);
    Mesh processMesh(aiMesh*, const aiScene*);

public:
    std::string directory;

    Model(const std::string& path);

    //void Draw(int shaderID) const;
    void Draw(int shaderID,
              glm::vec3    translation = glm::vec3(0.0f, 0.0f, 0.0f),
              glm::quat    rotation    = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
              glm::vec3    scale       = glm::vec3(1.0f, 1.0f, 1.0f)) const;
};
