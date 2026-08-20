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
        "Assets/shaders/geometry/model.vert",
        "Assets/shaders/geometry/model.frag",
        "Assets/shaders/geometry/model.geom");

    // PBR
    IDs.PBR = Load(
        "Assets/shaders/geometry/PBR.vert",
        "Assets/shaders/geometry/PBR.frag",
        "Assets/shaders/geometry/PBR.geom");

    // SkyBox
    IDs.skybox = Load(
        "Assets/shaders/skybox/skybox.vert",
        "Assets/shaders/skybox/skybox.frag");

    // postProcess
    IDs.postProcess = Load(
        "Assets/shaders/post-process/postProcess.vert",
        "Assets/shaders/post-process/postProcess.frag");

    // shadowMap2D
    IDs.shadowMap2D = Load(
        "Assets/shaders/lights/shadowMap2D.vert",
        "Assets/shaders/lights/shadowMap2D.frag");

    // shadowMapCube
    IDs.shadowMapCube = Load(
        "Assets/shaders/lights/shadowMapCube.vert",
        "Assets/shaders/lights/shadowMapCube.frag",
        "Assets/shaders/lights/shadowMapCube.geom");

    // Debug
    IDs.depthDebug = Load(
        "Assets/shaders/utility/debug.vert",
        "Assets/shaders/utility/debug.frag");

    // DebugCube
    IDs.depthDebugCube = Load(
        "Assets/shaders/utility/debug.vert",
        "Assets/shaders/utility/debugCube.frag");

    // Light
    IDs.lightSphere = Load(
        "Assets/shaders/lights/lightSphere.vert",
        "Assets/shaders/lights/lightSphere.frag");

    // postProcess
    IDs.blur = Load(
        "Assets/shaders/post-process/blur.vert",
        "Assets/shaders/post-process/blur.frag");

    // HDR texture converter
    IDs.HDRconverter = Load(
        "Assets/shaders/skybox/HDRtexConverter.vert",
        "Assets/shaders/skybox/HDRtexConverter.frag");
}
};  // namespace ShaderManager