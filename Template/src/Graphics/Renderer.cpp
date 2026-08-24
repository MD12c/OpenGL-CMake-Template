#include "Renderer.h"

#include "imgui.h"
#include "Globals.h"
#include "Lighting\LightSystem.h"

using namespace ShaderManager;

Renderer::Renderer()
    : postProcessSystem()
{
    LoadAllShaders();
    postProcessSystem.lut.Draw(ShaderIDs::BRDF_LUT);
}

Renderer::~Renderer()
{
    PrintLoadedUniforms();
    Cleanup();
}

void Renderer::Render(const Scene& scene)
{
    scene.lightSystem.ShadowPass(scene.models, scene.lights);

    postProcessSystem.Begin();

    auto useShader = ShaderIDs::PBR;
    scene.lightSystem.ExportUniforms(useShader, scene.lights);
    scene.skybox.ExportUniformsTo(useShader);
    postProcessSystem.lut.ExportUniformsTo(useShader);

    scene.cameras[scene.activeCam]->updateUniforms(useShader);
    for (const auto& model : scene.models)
        model.Draw(useShader);

    scene.cameras[scene.activeCam]->updateUniforms(ShaderIDs::LIGHT_SPHERE);
    scene.lightSystem.DrawLightSpheres(ShaderIDs::LIGHT_SPHERE, scene.lights);

    scene.skybox.Draw(ShaderIDs::SKYBOX, scene.cameras[scene.activeCam]->getRotationMat());

    postProcessSystem.End();

    scene.imguiFunctions();
}