#include "Model.h"

Model::Model(const std::string& path)
{
    loadModel(path);
}

void Model::Draw(const unsigned int shaderID) const
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(
            shaderID,
            glm::mat4(1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f));
    }
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
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // positions
        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);

        // normals
        if (mesh->HasNormals())
        {
            vertex.normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z);
        }
        else
        {
            vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        // texture coords
        if (mesh->mTextureCoords[0])
        {
            vertex.texUV = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.texUV = glm::vec2(0.0f, 0.0f);
        }

        // N/A texture color
        vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);

        vertices.push_back(vertex);
    }

    // indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // materials / textures
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // check if texture was already loaded
        bool skip = false;
        for (unsigned int j = 0; j < loadedTextures.size(); j++)
        {
            if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            std::string fullPath = directory + '/' + str.C_Str();
            Texture     texture(fullPath.c_str(), typeName.c_str(), (GLuint)loadedTextures.size());
            texture.path = str.C_Str();
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }

    return textures;
}