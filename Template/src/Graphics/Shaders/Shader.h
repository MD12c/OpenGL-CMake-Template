#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

#include "Globals.h"

std::string get_file_contents(const std::string& filename);

struct StringHash
{
    using is_transparent = void;

    std::size_t operator()(std::string_view sv) const noexcept
    {
        return std::hash<std::string_view>{}(sv);
    }
    std::size_t operator()(const std::string& s) const noexcept
    {
        return std::hash<std::string_view>{}(s);
    }
};

using u_map = std::unordered_map<std::string, GLint, StringHash, std::equal_to<>>;

class Shader
{
private:
    inline static ShaderID boundShader = LAST_SHADER;

    Shader(std::string name) : name(std::move(name)) {}
    void   Init(const std::vector<std::pair<GLenum, const std::string&>>& stages);
    GLuint CompileShaderStage(GLenum type, const std::string& sourcePath);
    void   PrintProgramError(GLuint program);

    u_map uniforms = {};
    u_map units    = {};

public:
    GLuint      ID   = 0;
    std::string name = "N/A";

    Shader() {}
    static Shader MakeVertFrag(std::string name, std::string vertexPath, std::string fragmentPath);
    static Shader MakeVertGeomFrag(std::string name, std::string vertexPath, std::string fragmentPath, std::string geometryPath);
    static Shader MakeCompute(std::string name, std::string computePath);

    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other) noexcept
    {
        if (this != &other)
        {
            glDeleteProgram(ID);
            ID             = other.ID;
            name           = other.name;
            uniforms       = other.uniforms;
            units          = other.units;
            other.units    = {};
            other.uniforms = {};
            other.ID       = 0;
            other.name     = "";
        }
        return *this;
    }
    Shader(Shader&& other) noexcept
    {
        ID             = other.ID;
        name           = other.name;
        uniforms       = other.uniforms;
        units          = other.units;
        other.units    = {};
        other.uniforms = {};
        other.ID       = 0;
        other.name     = "";
    }
    ~Shader() { glDeleteProgram(ID); }

    static u_map&   getUniforms(ShaderID index);
    static GLint    getUnit(ShaderID index, std::string_view name);
    static GLint    getLoc(ShaderID index, std::string_view uniformName);
    static ShaderID getShaderIDfromLightType(LightType type);

    static Shader& get(ShaderID ID);
    static void    PrintLoadedUniforms();
    static void    LoadAllShaders();
    static void    AddUnits(ShaderID index, u_map&& units);

    static void Activate(ShaderID index);
    static void Cleanup();
};

inline std::array<Shader, LAST_SHADER> shaders = {};

#endif
