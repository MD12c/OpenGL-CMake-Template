#include "Mesh.h"

#include <cstddef>

Mesh::Mesh(std::vector<Vertex>& verticies, std::vector<GLuint>& indices, int materialID)
    : vertices(verticies), indices(indices), materialID(materialID), VAO(), VBO(vertices), EBO(indices)
{
    VAO.Bind();
    VBO.Bind();
    EBO.Bind();

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::Draw(unsigned int shaderID, const glm::mat4& model, const glm::mat4& normal) const
{
    ShaderManager::Activate(shaderID);

    VAO.Bind();

    if (materialID >= 0)
        MaterialManager::getMatAt(materialID).Apply(shaderID);
    else
        MaterialManager::Unbind();

    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(ShaderManager::getLoc(shaderID, "normal"), 1, GL_FALSE, glm::value_ptr(normal));

    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0); // wireframe for debug
}
