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

    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, MaterialID materialID);

    Mesh(const Mesh&)                = delete;
    Mesh& operator=(const Mesh&)     = delete;
    Mesh(Mesh&&) noexcept            = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    void Draw(ShaderID shaderID, glm::mat4 model = glm::mat4(1.0f), glm::mat3 normal = glm::mat3(1.0f), MaterialID drawMaterialID = NO_MATERIAL) const;
    void DrawSimple(ShaderID shaderID) const;

    static std::vector<GLuint> makeVecIndex(const GLuint* array, size_t size);
    static std::vector<Vertex> makeVecVertex(const GLfloat* array, size_t size);
};