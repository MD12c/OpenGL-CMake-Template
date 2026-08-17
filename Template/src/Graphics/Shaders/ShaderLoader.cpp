#include "ShaderManager.h"

#include "Globals.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

namespace ShaderManager
{
ShaderIDs IDs;

int Load(const std::string& vertPath,
         const std::string& fragPath,
         const std::string& geomPath)
{
    shaderResources.emplace_back(ShaderResources(Shader(vertPath, fragPath, geomPath), {}));
    return (int)shaderResources.size() - 1;
}

int Load(const std::string& vertPath,
         const std::string& fragPath)
{
    shaderResources.emplace_back(ShaderResources(Shader(vertPath, fragPath), {}));
    return (int)shaderResources.size() - 1;
}

void LoadAllShaders()
{
    // Model 0
    IDs.model = Load(
        "Assets/shaders/model.vert",
        "Assets/shaders/model.frag",
        "Assets/shaders/model.geom");

    // SkyBox 1
    IDs.skybox = Load(
        "Assets/shaders/skybox.vert",
        "Assets/shaders/skybox.frag");

    // postProcess 2
    IDs.postProcess = Load(
        "Assets/shaders/postProcess.vert",
        "Assets/shaders/postProcess.frag");

    // shadowMap2D 3
    IDs.shadowMap2D = Load(
        "Assets/shaders/shadowMap2D.vert",
        "Assets/shaders/shadowMap2D.frag");

    // shadowMapCube 4
    IDs.shadowMapCube = Load(
        "Assets/shaders/shadowMapCube.vert",
        "Assets/shaders/shadowMapCube.frag",
        "Assets/shaders/shadowMapCube.geom");

    // Debug 5
    IDs.depthDebug = Load(
        "Assets/shaders/debug.vert",
        "Assets/shaders/debug.frag");

    // DebugCube 6
    IDs.depthDebugCube = Load(
        "Assets/shaders/debug.vert",
        "Assets/shaders/debugCube.frag");

    // Light 7
    IDs.lightSphere = Load(
        "Assets/shaders/lightSphere.vert",
        "Assets/shaders/lightSphere.frag");
    
    // postProcess 2
    IDs.blur = Load(
        "Assets/shaders/blur.vert",
        "Assets/shaders/blur.frag");

    // Default 8
    IDs.def = Load(
        "Assets/shaders/default.vert",
        "Assets/shaders/default.frag");
}
};  // namespace ShaderManager