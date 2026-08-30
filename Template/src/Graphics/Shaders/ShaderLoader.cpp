#include "Shader.h"

#include "Globals.h"

void Shader::LoadAllShaders()
{
    // Specular
    shaders.at(ShaderID::SPECULAR) = MakeVertGeomFrag(
        "Specular",
        "Assets/shaders/geometry/model.vert",
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
    shaders.at(ShaderID::PBR) = MakeVertGeomFrag(
        "PBR",
        "Assets/shaders/geometry/PBR.vert",
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
    shaders.at(ShaderID::SKYBOX) = MakeVertFrag(
        "SkyBox",
        "Assets/shaders/skybox/skybox.vert",
        "Assets/shaders/skybox/skybox.frag");
    AddUnits(ShaderID::SKYBOX, { { "skybox", 0 } });

    // postProcess
    shaders.at(ShaderID::POSTPROCESS) = MakeVertFrag(
        "postProcess",
        "Assets/shaders/post-process/postProcess.vert",
        "Assets/shaders/post-process/postProcess.frag");
    AddUnits(ShaderID::POSTPROCESS, { { "tex0", 0 }, { "tex1", 1 } });

    // shadowMap2D
    shaders.at(ShaderID::SHADOW_MAP2D) = MakeVertFrag(
        "shadowMap2D",
        "Assets/shaders/lights/shadowMap2D.vert",
        "Assets/shaders/lights/shadowMap2D.frag");

    // shadowMapCube
    shaders.at(ShaderID::SHADOW_MAPCUBE) = MakeVertGeomFrag(
        "shadowMapCube",
        "Assets/shaders/lights/shadowMapCube.vert",
        "Assets/shaders/lights/shadowMapCube.frag",
        "Assets/shaders/lights/shadowMapCube.geom");

    // Debug2D
    shaders.at(ShaderID::DEPTH_DEBUG2D) = MakeVertFrag(
        "Debug",
        "Assets/shaders/utility/debug.vert",
        "Assets/shaders/utility/debug.frag");

    // DebugCube
    shaders.at(ShaderID::DEPTH_DEBUGCUBE) = MakeVertFrag(
        "DebugCube",
        "Assets/shaders/utility/debug.vert",
        "Assets/shaders/utility/debugCube.frag");

    // Light
    shaders.at(ShaderID::LIGHT_SPHERE) = MakeVertFrag(
        "Light",
        "Assets/shaders/lights/lightSphere.vert",
        "Assets/shaders/lights/lightSphere.frag");

    // blur
    shaders.at(ShaderID::BLUR) = MakeVertFrag(
        "blur",
        "Assets/shaders/post-process/blur.vert",
        "Assets/shaders/post-process/blur.frag");
    AddUnits(ShaderID::BLUR, { { "tex0", 0 } });

    // HDR texture converter
    shaders.at(ShaderID::HDR_CONVERTER) = MakeVertFrag(
        "HDR texture converter",
        "Assets/shaders/skybox/HDRtexConverter.vert",
        "Assets/shaders/skybox/HDRtexConverter.frag");
    AddUnits(ShaderID::HDR_CONVERTER, { { "equirectangularMap", 0 } });

    // Irradiance
    shaders.at(ShaderID::IRRADIANCE) = MakeVertFrag(
        "Irradiance",
        "Assets/shaders/skybox/irradiance.vert",
        "Assets/shaders/skybox/irradiance.frag");
    AddUnits(ShaderID::IRRADIANCE, { { "environmentMap", 0 } });

    // Prefilter
    shaders.at(ShaderID::PREFILTER) = MakeVertFrag(
        "Prefilter",
        "Assets/shaders/skybox/prefilter.vert",
        "Assets/shaders/skybox/prefilter.frag");
    AddUnits(ShaderID::PREFILTER, { { "environmentMap", 0 } });

    // BRDF LUT
    shaders.at(ShaderID::BRDF_LUT) = MakeVertFrag(
        "BRDF LUT",
        "Assets/shaders/skybox/LUT.vert",
        "Assets/shaders/skybox/LUT.frag");

    // Default
    // shaders.at(ShaderID::DEFAULT) = MakeVertFrag(
    //     "BRDF LUT",
    //     "Assets/shaders/utility/default.vert",
    //     "Assets/shaders/utility/default.frag");
    // AddUnits(ShaderID::DEFAULT, { { "tex0", 0 } });
}