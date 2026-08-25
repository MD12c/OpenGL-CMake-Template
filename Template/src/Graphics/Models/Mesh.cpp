#include "Mesh.h"

#include <cstddef>

Mesh::Mesh(std::vector<Vertex>& verticies, std::vector<GLuint>& indices, MaterialID materialID)
    : vertices(verticies), indices(indices), materialID(materialID), VAO(), VBO(vertices), EBO(indices)
{
    VAO.Bind();
    VBO.Bind();
    EBO.Bind();

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
    VAO.LinkAttrib(VBO, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::Draw(ShaderID shaderID, glm::mat4 model, glm::mat3 normal, MaterialID drawMaterialID) const
{
    ShaderManager::Activate(shaderID);

    VAO.Bind();

    if (drawMaterialID == USE_FILE_MATERIAL)
        MaterialManager::getMatAt(materialID).Apply();
    else if (drawMaterialID == NO_MATERIAL)
        MaterialManager::Unbind(materialID);
    else
        MaterialManager::getMatAt(drawMaterialID).Apply();

    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(ShaderManager::getLoc(shaderID, "normal"), 1, GL_FALSE, glm::value_ptr(normal));

    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0); // wireframe for debug
}
