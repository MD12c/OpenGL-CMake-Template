#pragma once

#include <vector>

#include "../Shaders/Shader.h"
#include "../Frustum/BoundingSphere.h"

#include "../GLObjects/VAO.h"
#include "../GLObjects/EBO.h"
#include "../GLObjects/VBO.h"

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