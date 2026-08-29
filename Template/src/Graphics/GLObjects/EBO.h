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

    EBO(const EBO&)            = delete;
    EBO& operator=(const EBO&) = delete;

    EBO(EBO&& other) noexcept;
    EBO& operator=(EBO&& other) noexcept;

    ~EBO();

    void UpdateData(const void* indices, GLsizeiptr size);
    void UpdateData(std::vector<GLuint>& indices);
    void Bind() const;
    void Unbind() const;
};

#endif
