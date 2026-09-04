#include "UBO.h"

#include "../Shaders/Shader.h"

UBO::UBO(GLsizeiptr size, GLenum usageHint)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, usageHint);
    bindingIndex = nextBindingIndex++;
}

UBO& UBO::operator=(UBO&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &ID);

        ID                 = other.ID;
        bindingIndex       = other.bindingIndex;
        other.bindingIndex = 0;
        other.ID           = 0;
    }

    return *this;
}

UBO::UBO(UBO&& other) noexcept
    : ID(other.ID), bindingIndex(other.bindingIndex)
{
    other.ID           = 0;
    other.bindingIndex = 0;
}

UBO::~UBO()
{
    glDeleteBuffers(1, &ID);
}

void UBO::linkShader(ShaderID shaderID, const std::string& blockName)
{
    const auto& shader     = Shader::get(shaderID);
    GLuint      blockIndex = glGetUniformBlockIndex(shader.ID, blockName.c_str());
    glUniformBlockBinding(shader.ID, blockIndex, bindingIndex);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, ID);
}

void UBO::UpdateData(const void* data, GLsizeiptr size) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
}

void UBO::Bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
}

void UBO::Unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
