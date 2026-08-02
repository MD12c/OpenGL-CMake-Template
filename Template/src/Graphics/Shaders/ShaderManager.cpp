#include "ShaderManager.h"

namespace ShaderManager
{
std::unordered_map<std::string, ShaderResources> shaderResources;

ShaderResources* checkIfExists(const std::string& name)
{
    auto it = shaderResources.find(name);
    return it != shaderResources.end() ? &it->second : nullptr;
}

std::string Load(const std::string& name, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
{
    if (checkIfExists(name))
        return name;

    shaderResources.emplace(name, ShaderResources(Shader(vertPath, fragPath, geomPath), {}));
    return name;
}

std::string Load(const std::string& name, const std::string& vertPath, const std::string& fragPath)
{
    if (checkIfExists(name))
        return name;

    shaderResources.emplace(name, ShaderResources(Shader(vertPath, fragPath), {}));
    return name;
}

Shader& Get(const std::string& name)
{
    return shaderResources.at(name).shader;
}

void AddUniforms(const std::string& name, const std::unordered_map<std::string, GLint>& uniforms)
{
    auto& resource    = shaderResources.at(name);
    resource.uniforms = uniforms;
}

void AddUniform(const std::string& name, const std::string& uniformName, GLint uniformLoc)
{
    auto& resource = shaderResources.at(name);
    resource.uniforms.emplace(uniformName, uniformLoc);
}

GLint GetUniformLoc(const std::string& name, const std::string& uniformName)
{
    auto& resource = shaderResources.at(name);

    auto it = resource.uniforms.find(uniformName);
    if (it != resource.uniforms.end())
        return it->second;

    GLint loc                      = glGetUniformLocation(resource.shader.ID, uniformName.c_str());
    resource.uniforms[uniformName] = loc;
    return loc;
}

std::unordered_map<std::string, GLint>& GetUniforms(const std::string& name)
{
    return shaderResources.at(name).uniforms;
}

void Activate(const std::string& name)
{
    shaderResources.at(name).shader.Activate();
}

void Cleanup()
{
    shaderResources.clear();
}
};  // namespace ShaderManager