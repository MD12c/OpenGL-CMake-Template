#include "Shader.h"

#include <fstream>
#include <iostream>

Shader Shader::MakeVertFrag(std::string name, std::string vertexPath, std::string fragmentPath)
{
    Shader shader(std::move(name));
    shader.Init({ { GL_VERTEX_SHADER, vertexPath },
                  { GL_FRAGMENT_SHADER, fragmentPath } });
    return shader;
}

Shader Shader::MakeVertGeomFrag(std::string name, std::string vertexPath, std::string fragmentPath, std::string geometryPath)
{
    Shader shader(std::move(name));
    shader.Init({ { GL_VERTEX_SHADER, vertexPath },
                  { GL_FRAGMENT_SHADER, fragmentPath },
                  { GL_GEOMETRY_SHADER, geometryPath } });
    return shader;
}

Shader Shader::MakeCompute(std::string name, std::string computePath)
{
    Shader shader(std::move(name));
    shader.Init({ { GL_COMPUTE_SHADER, computePath } });
    return shader;
}

void Shader::Init(const std::vector<std::pair<GLenum, const std::string&>>& stages)
{
    ID = glCreateProgram();
    std::vector<GLuint> compiledShaders;
    compiledShaders.reserve(stages.size());

    for (const auto& [stageType, path] : stages)
    {
        GLuint shader = CompileShaderStage(stageType, path);
        glAttachShader(ID, shader);
        compiledShaders.push_back(shader);
    }

    glLinkProgram(ID);
    glObjectLabel(GL_PROGRAM, ID, -1, name.c_str());

    GLint success = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) PrintProgramError(ID);

    for (GLuint shader : compiledShaders)
    {
        glDetachShader(ID, shader);
        glDeleteShader(shader);
    }
}

GLuint Shader::CompileShaderStage(GLenum type, const std::string& sourcePath)
{
    std::string   sourceCode = get_file_contents(sourcePath);
    const GLchar* src        = sourceCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) PrintProgramError(shader);

    return shader;
}

std::string get_file_contents(const std::string& filename)
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

void Shader::PrintProgramError(GLuint program)
{
    char infoLog[1024];
    glGetProgramInfoLog(program, 1024, NULL, infoLog);
    std::cerr << "Shader error (" << name << "):\n"
              << infoLog << "\n";
}

// ---------------------------------------------------------
// Usage
// ---------------------------------------------------------

GLint Shader::getLoc(ShaderID index, std::string_view uniformName)
{
    auto& shader = get(index);

    auto it = shader.uniforms.find(uniformName);
    if (it != shader.uniforms.end())
        return it->second;

    std::string nameStr(uniformName);
    GLint       loc = glGetUniformLocation(shader.ID, nameStr.c_str());

    if (loc == -1)  // GLSL compiler strips unused uniforms
    {
        if (shader.name != "N/A")
            std::cerr << "[WARNING] uniform location not found,  Shader: " << shader.name << ",  Uniform Name: " << uniformName << "\n";
        else
            std::cerr << "[WARNING] uniform location not found,  Shader ID: " << std::to_string(shader.ID) << ",  Uniform Name: " << uniformName << "\n";
    }

    shader.uniforms.insert_or_assign(std::move(nameStr), loc);
    return loc;
}

ShaderID Shader::getShaderIDfromLightType(LightType type)
{
    if (type == LightType::DIRECTION)
        return ShaderID::SHADOW_MAP2D;
    else if (type == LightType::SPOT)
        return ShaderID::SHADOW_MAP2D;
    else if (type == LightType::POINT)
        return ShaderID::SHADOW_MAPCUBE;
    else
        throw std::runtime_error("[ERROR] Invalid light type");
};

GLint Shader::getUnit(ShaderID index, std::string_view name)
{
    const auto& unitMap = shaders.at(index).units;
    if (auto it = unitMap.find(name); it != unitMap.end())
        return it->second;
    throw std::out_of_range("Texture unit not found: " + std::string(name));
}

u_map& Shader::getUniforms(ShaderID index)
{
    return get(index).uniforms;
}

Shader& Shader::get(ShaderID ID)
{
    return shaders.at(static_cast<GLuint>(ID));
}

void Shader::Activate(ShaderID index)
{
    if (boundShader != index)
    {
        boundShader = index;
        glUseProgram(shaders.at(static_cast<GLuint>(index)).ID);
    }
}

void Shader::Cleanup()
{
    for (auto& resource : shaders)
        resource = Shader{};
}

void Shader::AddUnits(ShaderID index, u_map&& units)
{
    get(index).units = std::move(units);
}

// Old! opted for RenderDoc
void Shader::PrintLoadedUniforms()
{
    for (int i = 0; i < ShaderID::LAST_SHADER; i++)
    {
        Shader& shader   = get(static_cast<ShaderID>(i));
        GLint   shaderID = shader.ID;
        auto&   map      = shader.getUniforms(static_cast<ShaderID>(i));
        std::cout << shader.name << ":\n";

        size_t biggestString = 0;
        for (const auto& pair : map)
            biggestString = std::max(biggestString, pair.first.size());

        for (const auto& pair : map)
        {
            size_t spacing = biggestString - pair.first.size();
            std::cout << "   " << pair.first;
            for (int j = 0; j < spacing + 1; j++)
                std::cout << " ";

            GLint location = pair.second;
            if (location == -1)
            {
                std::cout << "unknown\n";
                continue;
            }

            std::string resourceName = pair.first;
            size_t      bracket      = resourceName.find('[');
            if (bracket != std::string::npos)
                resourceName.erase(bracket);

            GLuint resourceIndex = glGetProgramResourceIndex(shaderID, GL_UNIFORM, resourceName.c_str());
            if (resourceIndex == GL_INVALID_INDEX)
            {
                std::cout << "INVALID RESOURCE\n";
                continue;
            }

            GLenum  props[]   = { GL_TYPE, GL_ARRAY_SIZE };
            GLint   params[2] = {};
            GLsizei length    = 0;
            glGetProgramResourceiv(shaderID, GL_UNIFORM, resourceIndex, 2, props, 2, &length, params);
            GLenum type      = params[0];
            GLint  arraySize = params[1];

            std::string str;
            switch (type)
            {
                case GL_FLOAT:
                {
                    std::vector<GLfloat> values(arraySize);
                    glGetUniformfv(shaderID, location, values.data());
                    str = std::format("{}", values);
                    break;
                }
                case GL_FLOAT_VEC2:
                {
                    std::vector<GLfloat> values(2 * arraySize);
                    glGetUniformfv(shaderID, location, values.data());
                    str = std::format("{}", values);
                    break;
                }
                case GL_FLOAT_VEC3:
                {
                    std::vector<GLfloat> values(3 * arraySize);
                    glGetUniformfv(shaderID, location, values.data());
                    str = std::format("{}", values);
                    break;
                }
                case GL_FLOAT_VEC4:
                {
                    std::vector<GLfloat> values(4 * arraySize);
                    glGetUniformfv(shaderID, location, values.data());
                    str = std::format("{}", values);
                    break;
                }
                case GL_FLOAT_MAT4:
                {
                    std::vector<GLfloat> values(16);
                    glGetUniformfv(shaderID, location, values.data());
                    str = std::format("{}", values);
                    break;
                }
                case GL_SAMPLER_2D:
                case GL_SAMPLER_2D_ARRAY:
                case GL_SAMPLER_CUBE_MAP_ARRAY:
                case GL_SAMPLER_CUBE:
                case GL_INT:
                case GL_BOOL:
                {
                    std::vector<GLint> values(arraySize);
                    glGetUniformiv(shaderID, location, values.data());
                    str = std::format("{}", values);
                    break;
                }
                default:
                    str = "unknown";
            }
            std::cout << str << "\n";
        }
        std::cout << std::endl;
    }
}