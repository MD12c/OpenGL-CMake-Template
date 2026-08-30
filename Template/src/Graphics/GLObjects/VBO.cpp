#include "VBO.h"

VBO::VBO(const void* vertices, GLsizeiptr size, int usageHint)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usageHint);
}

VBO::VBO(const std::vector<Vertex>& vertices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VBO::VBO()
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

VBO& VBO::operator=(VBO&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &ID);

        ID       = other.ID;
        other.ID = 0;
    }

    return *this;
}

VBO::VBO(VBO&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

VBO::~VBO()
{
    glDeleteBuffers(1, &ID);
}

void VBO::UpdateData(const void* vertices, GLsizeiptr size)
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::UpdateData(std::vector<Vertex>& vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
