#include "ShaderManager.h"
#include <iostream>

namespace ShaderManager
{
std::vector<ShaderResources> shaderResources;

Shader& Get(unsigned int ID)
{
    return shaderResources.at(ID).shader;
}

void addUniforms(unsigned int ID, const std::unordered_map<std::string, GLint>& uniforms)
{
    auto& resource    = shaderResources.at(ID);
    resource.uniforms = uniforms;
}

void addUniform(unsigned int ID, const std::string& uniformName, GLint uniformLoc)
{
    auto& resource = shaderResources.at(ID);
    resource.uniforms.emplace(uniformName, uniformLoc);
}

GLint getLoc(unsigned int ID, const std::string& uniformName)
{
    auto& resource = shaderResources.at(ID);

    auto it = resource.uniforms.find(uniformName);
    if (it != resource.uniforms.end())
        return it->second;

    GLint loc = glGetUniformLocation(resource.shader.ID, uniformName.c_str());
    if (loc == -1)
        std::cerr << "[ERROR] uniform location not found, Shader ID: " + std::to_string(ID) << std::endl;
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