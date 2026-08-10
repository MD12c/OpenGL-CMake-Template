#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "..\GLObjects\VAO.h"
#include "..\GLObjects\EBO.h"
#include "../Cameras/Camera.h"
#include "..\GLObjects\Texture.h"
#include "../Shaders/ShaderManager.h"

class Mesh
{
public:
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;

    VAO VAO;

    mutable std::unordered_set<unsigned int> configuredShaders;

    Mesh(std::vector<Vertex>& verticies, std::vector<GLuint>& indices, std::vector<Texture>& textures);
    void addTextureUnits(unsigned int shaderID) const;

    void Draw(
        unsigned int shaderID,
        glm::mat4    model       = glm::mat4(1.0f),
        glm::vec3    translation = glm::vec3(1.0f),
        glm::quat    rotation    = glm::quat(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec3    scale       = glm::vec3(1.0f, 1.0f, 1.0f)) const;
};