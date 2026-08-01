#include "SSBO.h"

SSBO::SSBO(const void* data, GLsizeiptr size, int usageHint)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usageHint);
    if (data == nullptr)
        glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
}

void SSBO::UpdateSSBO(const void* data, GLsizeiptr size, GLsizeiptr offset)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
}

SSBO::SSBO()
{
    glGenBuffers(1, &ID);
}

void SSBO::Bind(unsigned int index)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ID);
}

void SSBO::Delete()
{
    glDeleteBuffers(1, &ID);
}