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
    int model;
    int skybox;
    int postProcess;
    int shadowMap2D;
    int shadowMapCube;
    int depthDebug;
    int depthDebugCube;
    int lightSphere;
    int def;
};
extern ShaderIDs IDs;

void    LoadAllShaders();
int     Load(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
int     Load(const std::string& vertPath, const std::string& fragPath);
Shader& Get(int ID);
void    PrintLoadedUniforms();

std::unordered_map<std::string, GLint>& getUniforms(int ID);
GLint                                   getLoc(int ID, const std::string& uniformName);
std::string                             getName(int ID);

void Activate(int ID);
void Cleanup();
};  // namespace ShaderManager

#endif