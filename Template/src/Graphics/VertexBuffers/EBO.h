#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
    GLuint ID;
    EBO(const void* indices, GLsizeiptr size);
    EBO(std::vector<GLuint>& indices);
    EBO();
    void UpdateData(const void* indices, GLsizeiptr size);
    void UpdateData(std::vector<GLuint>& indices);
    void Bind();
    void Unbind();
    void Delete();
};

#endif
#pragma once
