#ifndef SSBO_CLASS_H
#define SSBO_CLASS_H

#include <glad/glad.h>

class SSBO
{
    GLuint ID;

public:
    SSBO(const void* data, GLsizeiptr size, int usageHint);
    SSBO();
    void UpdateSSBO(const void* data, GLsizeiptr size, GLsizeiptr offset);
    void Bind(unsigned int index);
    void Delete();
};

#endif