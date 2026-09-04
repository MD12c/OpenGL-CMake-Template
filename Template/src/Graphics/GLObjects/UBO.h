#ifndef UBO_CLASS_H
#define UBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Globals.h"

class UBO
{
private:
    inline static GLuint nextBindingIndex = 0;

public:
    GLuint ID;
    GLuint bindingIndex;

    UBO(GLsizeiptr size, GLenum usageHint);

    UBO(const UBO&)            = delete;
    UBO& operator=(const UBO&) = delete;

    UBO(UBO&& other) noexcept;
    UBO& operator=(UBO&& other) noexcept;

    ~UBO();

    void linkShader(ShaderID shaderID, const std::string& blockName);
    void UpdateData(const void* data, GLsizeiptr size) const;
    void Bind() const;
    void Unbind() const;
};

#endif