#include "Model.h"

Model::Model(const std::string& path)
{
    loadModel(path);
}

void Model::Draw(unsigned int shaderID, glm::vec3 translation, glm::quat rotation, glm::vec3 scale) const
{
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 rot   = glm::mat4_cast(rotation);
    glm::mat4 sca   = glm::scale(glm::mat4(1.0f), scale);

    glm::mat4 model = trans * rot * sca;

    glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(model)));

    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shaderID, model, normal);
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    std::cout << "Loading model from: " << path << std::endl;

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<GLuint> indices(mesh->mNumFaces * 3);
    int                 materialID;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // positions
        vertices[i].position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);

        // normals
        if (mesh->HasNormals())
        {
            vertices[i].normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z);
        }
        else
        {
            vertices[i].normal = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        // texture coords
        if (mesh->mTextureCoords[0])
        {
            vertices[i].texUV = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices[i].texUV = glm::vec2(0.0f, 0.0f);
        }
    }

    // indices
    unsigned int index = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < 3; j++)
        {
            indices[index++] = face.mIndices[j];
        }
    }

    // materials / textures
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiString    diff;
        bool        diffSuccess = (material->GetTexture(aiTextureType_DIFFUSE, 0, &diff) == AI_SUCCESS);
        std::string diffPath    = diff.length ? directory + "/" + diff.C_Str() : "";

        aiString    spec;
        bool        specSuccess = (material->GetTexture(aiTextureType_SPECULAR, 0, &spec) == AI_SUCCESS);
        std::string specPath    = spec.length ? directory + "/" + spec.C_Str() : "";

        materialID = MaterialManager::LoadMaterial(std::string(material->GetName().C_Str()), diffPath, specPath);
    }

    std::cout << "mesh verts: " << mesh->mNumVertices << ", faces: " << mesh->mNumFaces << std::endl;
    return Mesh(vertices, indices, materialID);
}