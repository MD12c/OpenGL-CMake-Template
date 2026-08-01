#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>

std::string get_file_contents(const char* filename);
class Shader
{
public:
    GLuint ID;
    Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile);
    Shader(const char* vertexFile, const char* fragmentFile);
    Shader(const char* computeFile);
    void PrintError(GLuint shader);
    void Activate();
    void Delete();
};
#endif
