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

void Mesh::addTextureUnits(const std::string& shaderName)
{
    if (configuredShaders.count(shaderName))
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

        textures[i].texUnit(shaderName, (type + num).c_str());
    }
	configuredShaders.insert(shaderName);
}

void Mesh::Draw(
    const std::string& shaderName,
    glm::mat4          matrix,
    glm::vec3          translation,
    glm::quat          rotation,
    glm::vec3          scale)
{
    ShaderManager::Get(shaderName).Activate();
	addTextureUnits(shaderName);

    VAO.Bind();

	for (auto& tex : textures)
		tex.Bind();

    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot   = glm::mat4(1.0f);
    glm::mat4 sca   = glm::mat4(1.0f);

    trans = glm::translate(trans, translation);
    rot   = glm::mat4_cast(rotation);
    sca   = glm::scale(sca, scale);

    glUniformMatrix4fv(ShaderManager::GetUniformLoc(shaderName, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(ShaderManager::GetUniformLoc(shaderName, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
    glUniformMatrix4fv(ShaderManager::GetUniformLoc(shaderName, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
    glUniformMatrix4fv(ShaderManager::GetUniformLoc(shaderName, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}
