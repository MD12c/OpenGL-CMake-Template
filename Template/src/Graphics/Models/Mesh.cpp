#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
    Mesh::vertices = vertices;
    Mesh::indices  = indices;
    Mesh::textures = textures;

    VAO.Bind();
    VBO VBO(vertices);
    EBO EBO(indices);

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::addTextureUnits(unsigned int shaderID)
{
    if (configuredShaders.count(shaderID))
        return;

    unsigned int numDiffuse  = 0;
    unsigned int numSpecular = 0;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string num;
        std::string type = textures[i].type;
        if (type == "diffuse")
            num = std::to_string(numDiffuse++);
        else if (type == "specular")
            num = std::to_string(numSpecular++);

        textures[i].texUnit(shaderID, (type + num).c_str());
    }
    configuredShaders.insert(shaderID);
}

void Mesh::Draw(
    unsigned int shaderID,
    glm::mat4    model,
    glm::vec3    translation,
    glm::quat    rotation,
    glm::vec3    scale)
{
    ShaderManager::Get(shaderID).Activate();
    addTextureUnits(shaderID);

    VAO.Bind();

    for (auto& tex : textures)
        tex.Bind();

    glm::mat4 trans  = glm::mat4(1.0f);
    glm::mat4 rot    = glm::mat4(1.0f);
    glm::mat4 sca    = glm::mat4(1.0f);
    glm::mat3 normal = glm::mat3(1.0f);

    trans  = glm::translate(trans, translation);
    rot    = glm::mat4_cast(rotation);
    sca    = glm::scale(sca, scale);
    normal = glm::transpose(glm::inverse(glm::mat3(model)));

    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(ShaderManager::getLoc(shaderID, "normal"), 1, GL_FALSE, glm::value_ptr(normal));

    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}
