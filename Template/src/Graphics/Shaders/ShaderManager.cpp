#include "ShaderManager.h"
#include <iostream>

namespace ShaderManager
{
std::vector<ShaderResources> shaderResources;

Shader& Get(unsigned int ID)
{
    return shaderResources.at(ID).shader;
}

void addUniforms(unsigned int ID, const std::vector<std::string>& uniforms)
{
    auto& resource = shaderResources.at(ID);
    for (const auto& u : uniforms)
        resource.uniforms.insert({ u, getLoc(ID, u) });
}

void addUniform(unsigned int ID, const std::string& uniformName)
{
    auto& resource = shaderResources.at(ID);
    resource.uniforms.emplace(uniformName, getLoc(ID, uniformName));
}

GLint getLoc(unsigned int ID, const std::string& uniformName)
{
    auto& resource = shaderResources.at(ID);

    auto it = resource.uniforms.find(uniformName);
    if (it != resource.uniforms.end())
        return it->second;

    GLint loc = glGetUniformLocation(resource.shader.ID, uniformName.c_str());
    if (loc == -1)  // GLSL compiler strips unused uniforms
        std::cerr << "[WARNING] uniform location not found,  Shader ID: " << std::to_string(ID) << ",  Uniform Name: " << uniformName << std::endl;

    resource.uniforms[uniformName] = loc;
    return loc;
}

std::unordered_map<std::string, GLint>& getUniforms(unsigned int ID)
{
    return shaderResources.at(ID).uniforms;
}

void Activate(unsigned int ID)
{
    shaderResources.at(ID).shader.Activate();
}

void Cleanup()
{
    shaderResources.clear();
}
};  // namespace ShaderManager