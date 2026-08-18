#include "ShaderManager.h"

#include <iostream>
#include <format>

namespace ShaderManager
{
std::vector<ShaderResources> shaderResources;

Shader& Get(int ID)
{
    return shaderResources.at(ID).shader;
}

std::string getName(int ID)
{
    std::string& filename = Get(ID).filename;
    std::size_t  start    = filename.find_last_of("/\\") + 1;
    std::size_t  end      = filename.find('.', start);
    if (start != std::string::npos || end != std::string::npos)
        return filename.substr(start, end - start);
    else
        return "N/A";
}

GLint getLoc(int ID, const std::string& uniformName)
{
    auto& resource = shaderResources.at(ID);

    auto it = resource.uniforms.find(uniformName);
    if (it != resource.uniforms.end())
        return it->second;

    GLint loc = glGetUniformLocation(resource.shader.ID, uniformName.c_str());

    if (loc == -1)  // GLSL compiler strips unused uniforms
    {
        std::string name = getName(ID);
        if (name != "N/A")
            std::cerr << "[WARNING] uniform location not found,  Shader: " << name << ",  Uniform Name: " << uniformName << std::endl;
        else
            std::cerr << "[WARNING] uniform location not found,  Shader ID: " << std::to_string(ID) << ",  Uniform Name: " << uniformName << std::endl;
    }

    resource.uniforms[uniformName] = loc;
    return loc;
}

std::unordered_map<std::string, GLint>& getUniforms(int ID)
{
    return shaderResources.at(ID).uniforms;
}

void PrintLoadedUniforms()
{
    int i = 0;
    for (ShaderIDs* it = &IDs; it < &IDs + sizeof(ShaderIDs); it += sizeof(decltype(IDs.model)))
    {
        auto&       map        = getUniforms(i);
        std::string shaderName = getName(i);
        GLint       shaderID   = Get(i).ID;
        std::cout << shaderName << ":\n";

        size_t biggestString = 0;
        for (const auto& pair : map)
            biggestString = std::max(biggestString, pair.first.size());

        for (const auto& pair : map)
        {
            size_t spacing =  biggestString - pair.first.size();
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
        i++;
    }
}

void Activate(int ID)
{
    shaderResources.at(ID).shader.Activate();
}

void Cleanup()
{
    shaderResources.clear();
}
};  // namespace ShaderManager