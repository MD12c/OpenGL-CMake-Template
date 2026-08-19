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
    // Model
    IDs.model = Load(
        "Assets/shaders/model.vert",
        "Assets/shaders/model.frag",
        "Assets/shaders/model.geom");

    // PBR
    IDs.PBR = Load(
        "Assets/shaders/PBR.vert",
        "Assets/shaders/PBR.frag",
        "Assets/shaders/PBR.geom");

    // SkyBox
    IDs.skybox = Load(
        "Assets/shaders/skybox.vert",
        "Assets/shaders/skybox.frag");

    // postProcess
    IDs.postProcess = Load(
        "Assets/shaders/postProcess.vert",
        "Assets/shaders/postProcess.frag");

    // shadowMap2D
    IDs.shadowMap2D = Load(
        "Assets/shaders/shadowMap2D.vert",
        "Assets/shaders/shadowMap2D.frag");

    // shadowMapCube
    IDs.shadowMapCube = Load(
        "Assets/shaders/shadowMapCube.vert",
        "Assets/shaders/shadowMapCube.frag",
        "Assets/shaders/shadowMapCube.geom");

    // Debug
    IDs.depthDebug = Load(
        "Assets/shaders/debug.vert",
        "Assets/shaders/debug.frag");

    // DebugCube
    IDs.depthDebugCube = Load(
        "Assets/shaders/debug.vert",
        "Assets/shaders/debugCube.frag");

    // Light
    IDs.lightSphere = Load(
        "Assets/shaders/lightSphere.vert",
        "Assets/shaders/lightSphere.frag");

    // postProcess
    IDs.blur = Load(
        "Assets/shaders/blur.vert",
        "Assets/shaders/blur.frag");
}
};  // namespace ShaderManager