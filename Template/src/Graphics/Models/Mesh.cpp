#include "Mesh.h"

#include <cstddef>
#include "../Materials/MaterialManager.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, MaterialID materialID)
    : vertices(vertices), indices(indices), materialID(materialID), vao(), vbo(vertices), ebo(indices), sphere(computeBoundingSphere(vertices))
{
    vao.Bind();
    vbo.Bind();
    ebo.Bind();
    ebo.UpdateData((void*)indices.data(), indices.size() * sizeof(GLuint));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
    vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    vao.Unbind();
    vbo.Unbind();
    // ebo.Unbind();
}

void Mesh::Draw(ShaderID shaderID, glm::mat4 model, glm::mat3 normal, MaterialID drawMaterialID) const
{
    GPUInstrumentationTimer timerGPU("Draw Call");

    Shader::Activate(shaderID);

    vao.Bind();

    if (drawMaterialID == USE_FILE_MATERIAL)
        MaterialManager::getMatAt(materialID).Apply();
    else if (drawMaterialID == NO_MATERIAL)
        Texture::UnbindAll();
    else
        MaterialManager::getMatAt(drawMaterialID).Apply();

    glUniformMatrix4fv(Shader::getLoc(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(Shader::getLoc(shaderID, "normal"), 1, GL_FALSE, glm::value_ptr(normal));

    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0); // wireframe for debug
    vao.Unbind();
}

void Mesh::DrawSimple(ShaderID shaderID) const
{
    Shader::Activate(shaderID);
    vao.Bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

std::vector<GLuint> Mesh::makeVecIndex(const GLuint* array, size_t size)
{
    std::vector<GLuint> vec;
    for (size_t i = 0; i < size; i++)
        vec.push_back(array[i]);

    return vec;
}

std::vector<Vertex> Mesh::makeVecVertex(const GLfloat* array, size_t size)
{
    std::vector<Vertex> vec;
    for (size_t i = 0; i < size; i += 11)
    {
        Vertex vertex;
        vertex.position = glm::vec3(array[i], array[i + 1], array[i + 2]);
        vertex.normal   = glm::vec3(array[i + 3], array[i + 4], array[i + 5]);
        vertex.texUV    = glm::vec2(array[i + 6], array[i + 7]);
        vertex.tangent  = glm::vec3(array[i + 8], array[i + 9], array[i + 10]);
        vec.push_back(vertex);
    }
    return vec;
}
