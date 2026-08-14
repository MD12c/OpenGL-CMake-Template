#ifndef SSBO_CLASS_H
#define SSBO_CLASS_H

#include <glad/glad.h>

class SSBO
{
public:
    GLuint ID;

    SSBO(const void* data, GLsizeiptr size, int usageHint);
    SSBO();

    SSBO(const SSBO&)            = delete;
    SSBO& operator=(const SSBO&) = delete;

    SSBO(SSBO&& other) noexcept;
    SSBO& operator=(SSBO&& other) noexcept;

    ~SSBO();
    void UpdateSSBO(const void* data, GLsizeiptr size, GLsizeiptr offset);
    void Bind(unsigned int index);
};

#endif