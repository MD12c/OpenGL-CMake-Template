#include "Mesh.h"

#include <cstddef>

#include "../Frustum/Frustum.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, MaterialID materialID)
    : vertices(vertices), indices(indices), materialID(materialID), vao(), vbo(vertices), ebo(indices), sphere(computeBoundingSphere(vertices))
{
    vao.Bind();
    vbo.Bind();
    ebo.Bind();

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
    vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Mesh::Draw(ShaderID shaderID, glm::mat4 model, glm::mat3 normal, MaterialID drawMaterialID) const
{
    GPUInstrumentationTimer timerGPU("Draw Call");

    ShaderManager::Activate(shaderID);

    vao.Bind();

    if (drawMaterialID == USE_FILE_MATERIAL)
        MaterialManager::getMatAt(materialID).Apply();
    else if (drawMaterialID == NO_MATERIAL)
        Texture::UnbindAll();
    else
        MaterialManager::getMatAt(drawMaterialID).Apply();

    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(ShaderManager::getLoc(shaderID, "normal"), 1, GL_FALSE, glm::value_ptr(normal));

    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0); // wireframe for debug
}
