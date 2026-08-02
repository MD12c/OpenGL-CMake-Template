#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>

std::string get_file_contents(const std::string& filename);
class Shader
{
public:
    GLuint ID;
    Shader(const std::string& vertexFile, const std::string& fragmentFile, const std::string& geometryFile);
    Shader(const std::string& vertexFile, const std::string& fragmentFile);
    Shader(const std::string& computeFile);
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept
    {
        ID       = other.ID;
        other.ID = 0;
    }
    ~Shader();
    void PrintError(GLuint shader);
    void Activate();
    void Delete();
};
#endif
