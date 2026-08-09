#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};

class VBO
{
public:
    GLuint ID;
    VBO(const void* vertices, GLsizeiptr size, int usageHint);
    VBO(std::vector<Vertex>& vertices);
    VBO();

    void UpdateData(const void* vertices, GLsizeiptr size);
    void UpdateData(std::vector<Vertex>& vertices);
    void Bind() const;
    void Unbind() const;
    void Delete() const;
};

#endif