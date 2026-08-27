#include "IBO.h"

IBO::IBO(const std::vector<DrawElementsIndirectCommand>& commands, GLenum usage)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, ID);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, commands.size() * sizeof(DrawElementsIndirectCommand), commands.data(), usage);
}

IBO::IBO() : ID(0)
{
}

IBO::IBO(IBO&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

IBO& IBO::operator=(IBO&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &ID);
        ID       = other.ID;
        other.ID = 0;
    }
    return *this;
}

IBO::~IBO()
{
    glDeleteBuffers(1, &ID);
}

void IBO::UpdateData(const std::vector<DrawElementsIndirectCommand>& commands)
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, ID);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, commands.size() * sizeof(DrawElementsIndirectCommand), commands.data(), GL_DYNAMIC_DRAW);
}

void IBO::Bind() const
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, ID);
}

void IBO::Unbind() const
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}