#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
    GLuint ID;
    VAO();

    VAO(const VAO&)            = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&& other) noexcept;
    VAO& operator=(VAO&& other) noexcept;

    ~VAO();

    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset);
    void Bind() const;
    void Unbind() const;
};

#endif