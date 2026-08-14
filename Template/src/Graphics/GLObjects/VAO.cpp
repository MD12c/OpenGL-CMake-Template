#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset)
{
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
}

VAO& VAO::operator=(VAO&& other) noexcept
{
    if (this != &other)
    {
        glDeleteVertexArrays(1, &ID);

        ID       = other.ID;
        other.ID = 0;
    }

    return *this;
}

VAO::VAO(VAO&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &ID);
}

void VAO::Bind() const
{
    glBindVertexArray(ID);
}

void VAO::Unbind() const
{
    glBindVertexArray(0);
}
