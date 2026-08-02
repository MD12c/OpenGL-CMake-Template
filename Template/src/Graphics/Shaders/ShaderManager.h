#ifndef SHADER_MANAGER_CLASS_H
#define SHADER_MANAGER_CLASS_H

#include <vector>
#include <unordered_map>
#include <optional>

#include "Shader.h"

namespace ShaderManager
{
struct ShaderResources
{
    Shader                                 shader;
    std::unordered_map<std::string, GLint> uniforms;
    ShaderResources(Shader shader, std::unordered_map<std::string, GLint> uniforms) : shader(std::move(shader)), uniforms(std::move(uniforms)) {}
    ~ShaderResources() { shader.Delete(); }
    ShaderResources(ShaderResources&&) noexcept            = default;
    ShaderResources& operator=(ShaderResources&&) noexcept = default;
    ShaderResources(const ShaderResources&)                = delete;
    ShaderResources& operator=(const ShaderResources&)     = delete;
};
extern std::unordered_map<std::string, ShaderResources> shaderResources;

std::string      Load(const std::string& name, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
std::string      Load(const std::string& name, const std::string& vertPath, const std::string& fragPath);
Shader&          Get(const std::string& name);
ShaderResources* checkIfExists(const std::string& name);

void                                    AddUniforms(const std::string& name, const std::unordered_map<std::string, GLint>& uniforms);
void                                    AddUniform(const std::string& name, const std::string& uniformName, GLint uniformLoc);
std::unordered_map<std::string, GLint>& GetUniforms(const std::string& name);
GLint                                   GetUniformLoc(const std::string& name, const std::string& uniformName);

void  Activate(const std::string& name);
GLint GetID(const std::string& name);
void  Cleanup();
};  // namespace ShaderManager

#endif