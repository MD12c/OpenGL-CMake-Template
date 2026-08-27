#ifndef IBO_CLASS_H
#define IBO_CLASS_H

#include <glad/glad.h>
#include <vector>

struct DrawElementsIndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

class IBO
{
public:
    GLuint ID;

    IBO(const std::vector<DrawElementsIndirectCommand>& commands, GLenum usage = GL_DYNAMIC_DRAW);
    IBO();

    IBO(const IBO&)            = delete;
    IBO& operator=(const IBO&) = delete;

    IBO(IBO&& other) noexcept;
    IBO& operator=(IBO&& other) noexcept;

    ~IBO();

    void UpdateData(const std::vector<DrawElementsIndirectCommand>& commands);
    void Bind() const;
    void Unbind() const;
};

#endif