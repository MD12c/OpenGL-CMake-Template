#include "ShaderManager.h"

#include <iostream>

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
        auto&       map  = getUniforms(i);
        std::string name = getName(i);
        std::cout << name << ":\n";
        for (const auto& pair : map)
            std::cout <<  "   " << pair.first << "\n";
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