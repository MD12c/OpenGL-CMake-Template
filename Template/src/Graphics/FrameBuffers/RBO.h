#ifndef RBO_CLASS_H
#define RBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class RBO
{
public:
    GLuint ID;
    RBO(bool useMultisample);

    void Bind();
    void Unbind();
    void Delete();
};

#endif