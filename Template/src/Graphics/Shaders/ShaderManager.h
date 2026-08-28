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
extern std::deque<ShaderResources> shaderResources;  // TODO: make array

extern ShaderID boundShader;

void    LoadAllShaders();
void    Load(const std::string& name, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
void    Load(const std::string& name, const std::string& vertPath, const std::string& fragPath);
Shader& Get(ShaderID ID);
void    AddUnits(ShaderID ID, std::unordered_map<std::string, GLint>&& units);
void    PrintLoadedUniforms();

std::unordered_map<std::string, GLint>& getUniforms(ShaderID ID);
GLint                                   getLoc(ShaderID ID, const std::string& uniformName);
GLint                                   getUnit(ShaderID ID, const std::string& name);
std::string                             getName(ShaderID ID);
ShaderID                                getShaderIDfromLightType(LightType type);

void Activate(ShaderID ID);
void Cleanup();
};  // namespace ShaderManager

#endif