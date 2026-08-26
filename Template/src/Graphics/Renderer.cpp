#include "Renderer.h"

#include "imgui.h"
#include "Globals.h"
#include "Lighting\LightSystem.h"

using namespace ShaderManager;

Renderer::Renderer()
    : postProcessSystem()
{
    LoadAllShaders();
    postProcessSystem.lut.Draw(ShaderID::BRDF_LUT);
}

Renderer::~Renderer()
{
    PrintLoadedUniforms();
    Cleanup();
}

void Renderer::Render(const Scene& scene)
{
    GPUInstrumentationTimer timer("Frame");
    scene.lightSystem.ShadowPass(scene.models, scene.lights);

    postProcessSystem.Begin();

    auto useShader = ShaderID::PBR;
    scene.lightSystem.ExportUniforms(useShader, scene.lights);
    scene.skybox.ExportUniformsTo(useShader);
    postProcessSystem.lut.ExportUniformsTo(useShader);

    scene.cameras[scene.activeCam]->updateUniforms(useShader);
    for (const auto& model : scene.models)
        model.Draw(useShader, {{}, {}, glm::vec3(0.02f)});

    scene.cameras[scene.activeCam]->updateUniforms(ShaderID::LIGHT_SPHERE);
    scene.lightSystem.DrawLightSpheres(ShaderID::LIGHT_SPHERE, scene.lights);

    scene.skybox.Draw(ShaderID::SKYBOX, scene.cameras[scene.activeCam]->getRotationMat());

    postProcessSystem.End();

    scene.imguiFunctions();
}