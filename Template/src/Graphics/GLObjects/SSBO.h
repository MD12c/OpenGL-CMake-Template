#ifndef SSBO_CLASS_H
#define SSBO_CLASS_H

#include <glad/glad.h>

class SSBO
{
public:
    GLuint ID;

    SSBO(const void* data, GLsizeiptr size, int usageHint);
    SSBO();
    ~SSBO();
    void UpdateSSBO(const void* data, GLsizeiptr size, GLsizeiptr offset);
    void Bind(unsigned int index);
};

#endif