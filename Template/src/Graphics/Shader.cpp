#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw(errno);
}

void Shader::PrintError(GLuint shader)
{
    GLint shaderType = 0;
    glGetShaderiv(shader, GL_SHADER_TYPE, &shaderType);

    std::string shaderName;
    if (shaderType == GL_VERTEX_SHADER)
    {
        shaderName = "Vertex";
    }
    else if (shaderType == GL_FRAGMENT_SHADER)
    {
        shaderName = "Fragment";
    }
    else if (shaderType == GL_GEOMETRY_SHADER)
    {
        shaderName = "Geometry";
    }
    else if (shaderType == GL_COMPUTE_SHADER)
    {
        shaderName = "Compute";
    }

    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << shaderName << " shader compilation failed:\n"
              << infoLog << "\n";
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertexCode   = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource   = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint successFrag;
    GLint successVert;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);

    if (!successVert)
        PrintError(vertexShader);
    if (!successFrag)
        PrintError(fragmentShader);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{
    std::string vertexCode   = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);
    std::string geometryCode = get_file_contents(geometryFile);

    const char* vertexSource   = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();
    const char* geometrySource = geometryCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometrySource, NULL);
    glCompileShader(geometryShader);

    GLint successFrag;
    GLint successVert;
    GLint successGeom;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &successGeom);

	if (!successVert)
        PrintError(vertexShader);
    if (!successFrag)
        PrintError(fragmentShader);
	if (!successGeom)
        PrintError(geometryShader);


    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glAttachShader(ID, geometryShader);

    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
}

Shader::Shader(const char* computeFile)
{
    std::string computeCode   = get_file_contents(computeFile);
    const char* computeSource = computeCode.c_str();
    GLuint      computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeSource, NULL);
    glCompileShader(computeShader);

    GLint successCompute;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &successCompute);
	
	if (!successCompute)
        PrintError(computeShader);

    ID = glCreateProgram();
    glAttachShader(ID, computeShader);
    glLinkProgram(ID);
    glDeleteShader(computeShader);
}

void Shader::Activate()
{
    glUseProgram(ID);
}
void Shader::Delete()
{
    glDeleteProgram(ID);
}