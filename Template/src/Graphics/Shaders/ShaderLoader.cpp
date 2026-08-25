#include "ShaderManager.h"

#include "Globals.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

namespace ShaderManager
{
void Load(const std::string& vertPath,
          const std::string& fragPath,
          const std::string& geomPath)
{
    shaderResources.emplace_back(ShaderResources(Shader(vertPath, fragPath, geomPath)));
}

void Load(const std::string& vertPath,
          const std::string& fragPath)
{
    shaderResources.emplace_back(ShaderResources(Shader(vertPath, fragPath)));
}

void Load(const std::string& compPath)
{
    shaderResources.emplace_back(ShaderResources(Shader(compPath)));
}

void LoadAllShaders()  //! load order matters see Globals.h ShaderID struct
{
    // Specular
    Load("Assets/shaders/geometry/model.vert",
         "Assets/shaders/geometry/model.frag",
         "Assets/shaders/geometry/model.geom");
    AddUnits(ShaderID::SPECULAR,
             { { "diffuse0", 0 },
               { "specular0", 1 },
               { "normal0", 2 },
               { "displacement0", 3 },
               { "dirShadowMaps", 4 },
               { "spotShadowMaps", 5 },
               { "pointShadowMaps", 6 } });

    // PBR
    Load("Assets/shaders/geometry/PBR.vert",
         "Assets/shaders/geometry/PBR.frag",
         "Assets/shaders/geometry/PBR.geom");
    AddUnits(ShaderID::PBR,
             { { "brdfLUT", 0 },
               { "albedo0", 1 },
               { "ao0", 2 },
               { "metalicRoughness0", 3 },
               { "normal0", 4 },
               { "displacement0", 5 },
               { "irradiance0", 6 },
               { "prefilteredmap0", 7 },
               { "dirShadowMaps", 8 },
               { "spotShadowMaps", 9 },
               { "pointShadowMaps", 10 } });

    // SkyBox
    Load("Assets/shaders/skybox/skybox.vert",
         "Assets/shaders/skybox/skybox.frag");

    // postProcess
    Load("Assets/shaders/post-process/postProcess.vert",
         "Assets/shaders/post-process/postProcess.frag");

    // shadowMap2D
    Load("Assets/shaders/lights/shadowMap2D.vert",
         "Assets/shaders/lights/shadowMap2D.frag");

    // shadowMapCube
    Load("Assets/shaders/lights/shadowMapCube.vert",
         "Assets/shaders/lights/shadowMapCube.frag",
         "Assets/shaders/lights/shadowMapCube.geom");

    // Debug
    Load("Assets/shaders/utility/debug.vert",
         "Assets/shaders/utility/debug.frag");

    // DebugCube
    Load("Assets/shaders/utility/debug.vert",
         "Assets/shaders/utility/debugCube.frag");

    // Light
    Load("Assets/shaders/lights/lightSphere.vert",
         "Assets/shaders/lights/lightSphere.frag");

    // postProcess
    Load("Assets/shaders/post-process/blur.vert",
         "Assets/shaders/post-process/blur.frag");
    AddUnits(ShaderID::POSTPROCESS, { { "tex0", 0 }, { "tex1", 1 } });

    // HDR texture converter
    Load("Assets/shaders/skybox/HDRtexConverter.vert",
         "Assets/shaders/skybox/HDRtexConverter.frag");
    AddUnits(ShaderID::HDR_CONVERTER, { { "equirectangularMap", 0 } });

    // Irradiance
    Load("Assets/shaders/skybox/irradiance.vert",
         "Assets/shaders/skybox/irradiance.frag");
    AddUnits(ShaderID::IRRADIANCE, { { "environmentMap", 0 } });

    // Prefilter
    Load("Assets/shaders/skybox/prefilter.vert",
         "Assets/shaders/skybox/prefilter.frag");

    // BRDF LUT
    Load("Assets/shaders/skybox/LUT.vert",
         "Assets/shaders/skybox/LUT.frag");
}
};  // namespace ShaderManager