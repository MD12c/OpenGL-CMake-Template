#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "../Shaders/Shader.h"
#include "../Materials/MaterialManager.h"
#include "../Frustum/BoundingSphere.h"

#include "../GLObjects/VAO.h"
#include "../GLObjects/EBO.h"
#include "../GLObjects/VBO.h"
#include "../GLObjects/Texture.h"

class Frustum;

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    int                 materialID;

    VAO vao;
    VBO vbo;
    EBO ebo;

    BoundingSphere sphere;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, MaterialID materialID);

    void Draw(ShaderID shaderID, glm::mat4 model, glm::mat3 normal, MaterialID drawMaterialID = NO_MATERIAL) const;
};