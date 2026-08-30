#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
    glm::vec3 position{};
    glm::vec3 normal{};
    glm::vec2 texUV{};
    glm::vec3 tangent{};
};

class VBO
{
public:
    GLuint ID;
    VBO(const void* vertices, GLsizeiptr size, int usageHint = GL_STATIC_DRAW);
    VBO(const std::vector<Vertex>& vertices);
    VBO();

    VBO(const VBO&)            = delete;
    VBO& operator=(const VBO&) = delete;

    VBO(VBO&& other) noexcept;
    VBO& operator=(VBO&& other) noexcept;

    ~VBO();

    void UpdateData(const void* vertices, GLsizeiptr size);
    void UpdateData(std::vector<Vertex>& vertices);
    void Bind() const;
    void Unbind() const;
};

#endif