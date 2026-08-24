#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../GLObjects/VAO.h"
#include "../GLObjects/EBO.h"
#include "../Cameras/Camera.h"
#include "../GLObjects/Texture.h"
#include "../Shaders/ShaderManager.h"
#include "../Materials/MaterialManager.h"

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    int                 materialID;

    VAO VAO;
    VBO VBO;
    EBO EBO;

    Mesh(std::vector<Vertex>& verticies, std::vector<GLuint>& indices, int materialID);

    void Draw(ShaderIDs shaderID,
              const glm::mat4&         model  = glm::mat4(1.0f),
              const glm::mat3&         normal = glm::mat3(1.0f)) const;
};