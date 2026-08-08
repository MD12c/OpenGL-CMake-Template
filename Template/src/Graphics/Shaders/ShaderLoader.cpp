#include "ShaderManager.h"

#include "Globals.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

namespace ShaderManager
{
ShaderIDs IDs;

unsigned int Load(const std::string& vertPath,
                  const std::string& fragPath,
                  const std::string& geomPath)
{
    shaderResources.emplace_back(ShaderResources(Shader(vertPath, fragPath, geomPath), {}));
    return (unsigned int)shaderResources.size() - 1;
}

unsigned int Load(const std::string& vertPath,
                  const std::string& fragPath)
{
    shaderResources.emplace_back(ShaderResources(Shader(vertPath, fragPath), {}));
    return (unsigned int)shaderResources.size() - 1;
}

unsigned int LoadWithUniforms(const std::string&              vertPath,
                              const std::string&              fragPath,
                              const std::vector<std::string>& uniformNames)
{
    unsigned int ID = Load(vertPath, fragPath);

    for (const auto& uniform : uniformNames)
        addUniform(ID, uniform);

    return ID;
}

unsigned int LoadWithUniforms(const std::string&              vertPath,
                              const std::string&              fragPath,
                              const std::string&              geomPath,
                              const std::vector<std::string>& uniformNames)
{
    unsigned int ID = Load(vertPath, fragPath, geomPath);

    for (const auto& uniform : uniformNames)
        addUniform(ID, uniform);

    return ID;
}

void LoadAllShaders()
{
    // Model 0
    IDs.model = LoadWithUniforms(
        "Assets/shaders/model.vert",
        "Assets/shaders/model.frag",
        "Assets/shaders/model.geom",
        { "proj",
          "view",
          "model",
          "translation",
          "rotation",
          "scale" });

    addUniforms(
        IDs.model,
        { // fagment shader uniforms
          "diffuse0",
          "specular0",
          "camPos",
          "numDirLights",
          "numSpotLights",
          "numPointLights" });

    Activate(IDs.model);
    // glUniform4f(getLoc(IDs.model, "lightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    // glUniform3fv(getLoc(IDs.model, "lightPos"), 1, glm::value_ptr(lightPosition));
    // glUniform3f(getLoc(IDs.model, "lightDirection"), 0.0f, -1.0f, 0.0f);

    // SkyBox 1
    IDs.skybox = LoadWithUniforms(
        "Assets/shaders/skybox.vert",
        "Assets/shaders/skybox.frag",
        { "cameraMatrix",
          "skybox" });

    Activate(IDs.skybox);
    glUniform1i(getLoc(IDs.skybox, "skybox"), 0);

    // postProcess 2
    IDs.postProcess = LoadWithUniforms(
        "Assets/shaders/postProcess.vert",
        "Assets/shaders/postProcess.frag",
        { "screenTexture",
          "gamma" });

    Activate(IDs.postProcess);
    glUniform1i(getLoc(IDs.postProcess, "screenTexture"), 0);
    glUniform1f(getLoc(IDs.postProcess, "gamma"), gamma);

    // shadowMap2D 3
    IDs.shadowMap2D = LoadWithUniforms(
        "Assets/shaders/shadowMap2D.vert",
        "Assets/shaders/shadowMap2D.frag",
        {
            "proj",
            "model",
            "view",
        });

    // shadowMapCube 4
    IDs.shadowMapCube = LoadWithUniforms(
        "Assets/shaders/shadowMapCube.vert",
        "Assets/shaders/shadowMapCube.frag",
        "Assets/shaders/shadowMapCube.geom",
        { "model",
          "lightPos",
          "farPlane" });
    for (int i = 0; i < 6; i++)
    {
        addUniform(
            IDs.shadowMapCube,
            "shadowMatrices[" + std::to_string(i) + "]");
    }

    // Debug 5
    IDs.depthDebug = LoadWithUniforms(
        "Assets/shaders/debug.vert",
        "Assets/shaders/debug.frag",
        { "depthMap" });
    Activate(IDs.depthDebug);
    glUniform1i(getLoc(IDs.depthDebug, "depthMap"), 0);

    // DebugCube 6
    IDs.depthDebugCube = LoadWithUniforms(
        "Assets/shaders/debug.vert",
        "Assets/shaders/debugCube.frag",
        {
            "depthCubeMap",
            "faceForward",
            "faceRight",
            "faceUp",
        });

    // Default 7
    IDs.def = LoadWithUniforms(
        "Assets/shaders/default.vert",
        "Assets/shaders/default.frag",
        {
            "proj",
            "view",
            "camPos",
            "Color",
            "useTexture",
            "tex0",
        });
    Activate(IDs.def);
    glUniform3fv(getLoc(IDs.def, "Color"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
}
};  // namespace ShaderManager