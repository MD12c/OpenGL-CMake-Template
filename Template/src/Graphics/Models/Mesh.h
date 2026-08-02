#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../VertexBuffers/VAO.h"
#include "../VertexBuffers/EBO.h"
#include "../Cameras/Camera.h"
#include "../FrameBuffers/Texture.h"
#include "../Shaders/ShaderManager.h"

class Mesh
{
public:
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;

    VAO VAO;

	std::unordered_set<std::string> configuredShaders;

    Mesh(std::vector<Vertex>& verticies, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	void addTextureUnits(const std::string& shaderName);

    void Draw(
        const std::string& shaderName,
        glm::mat4          matrix      = glm::mat4(1.0f),
        glm::vec3          translation = glm::vec3(1.0f),
        glm::quat          rotation    = glm::quat(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec3          scale       = glm::vec3(1.0f, 1.0f, 1.0f));
};