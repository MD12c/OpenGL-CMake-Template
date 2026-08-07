#ifndef SHADER_MANAGER_CLASS_H
#define SHADER_MANAGER_CLASS_H

#include <unordered_map>
#include <vector>

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
extern std::vector<ShaderResources> shaderResources;

struct ShaderIDs
{
    unsigned int model;
    unsigned int skybox;
    unsigned int postProcess;
    unsigned int shadowMap2D;
    unsigned int shadowMapCube;
    unsigned int depthDebug;
    unsigned int depthDebugCube;
    unsigned int def;
};
extern ShaderIDs IDs;

void         LoadAllShaders();
unsigned int LoadWithUniforms(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath, const std::vector<std::string>& uniformNames);
unsigned int LoadWithUniforms(const std::string& vertPath, const std::string& fragPath, const std::vector<std::string>& uniformNames);
unsigned int Load(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
unsigned int Load(const std::string& vertPath, const std::string& fragPath);
Shader&      Get(unsigned int ID);

void                                    addUniforms(unsigned int ID, const std::unordered_map<std::string, GLint>& uniforms);
void                                    addUniform(unsigned int ID, const std::string& uniformName, GLint uniformLoc);
std::unordered_map<std::string, GLint>& getUniforms(unsigned int ID);
GLint                                   getLoc(unsigned int ID, const std::string& uniformName);

void Activate(unsigned int ID);
void Cleanup();
};  // namespace ShaderManager

#endif