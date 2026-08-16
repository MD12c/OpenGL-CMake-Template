#ifndef RBO_CLASS_H
#define RBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class RBO
{
public:
    GLuint ID;
    bool useMultisample;

    RBO(bool useMultisample);
    
    RBO(const RBO&)            = delete;
    RBO& operator=(const RBO&) = delete;

    RBO(RBO&& other) noexcept;
    RBO& operator=(RBO&& other) noexcept;

    ~RBO();

    void Bind();
    void Unbind();
    void Delete();
};

#endif