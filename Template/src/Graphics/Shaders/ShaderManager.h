#ifndef SHADER_MANAGER_CLASS_H
#define SHADER_MANAGER_CLASS_H

#include <unordered_map>
#include <deque>

#include "Globals.h"
#include "Shader.h"

namespace ShaderManager
{
struct ShaderResources
{
    Shader                                 shader;
    std::unordered_map<std::string, GLint> uniforms = {};
    std::unordered_map<std::string, GLint> units    = {};

    ShaderResources(Shader shader) : shader(std::move(shader)) {}
    ~ShaderResources() { shader.Delete(); }
    ShaderResources(ShaderResources&&) noexcept            = default;
    ShaderResources& operator=(ShaderResources&&) noexcept = default;
    ShaderResources(const ShaderResources&)                = delete;
    ShaderResources& operator=(const ShaderResources&)     = delete;
};
extern std::deque<ShaderResources> shaderResources;

void    LoadAllShaders();
void    Load(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
void    Load(const std::string& vertPath, const std::string& fragPath);
Shader& Get(ShaderIDs ID);
void    AddUnits(ShaderIDs ID, std::unordered_map<std::string, GLint>&& units);
void    PrintLoadedUniforms();

std::unordered_map<std::string, GLint>& getUniforms(ShaderIDs ID);
GLint                                   getLoc(ShaderIDs ID, const std::string& uniformName);
GLint                                   getUnit(ShaderIDs ID, const std::string& name);
std::string                             getName(ShaderIDs ID);
ShaderIDs                               getShaderIDfromType(LightType type);

void Activate(ShaderIDs ID);
void Cleanup();
};  // namespace ShaderManager

#endif