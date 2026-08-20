#include "Renderer.h"

#include "imgui.h"
#include "Globals.h"
#include "Lighting\LightSystem.h"

using namespace ShaderManager;

Renderer::Renderer()

{
    LoadAllShaders();
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

    int useShader = IDs.PBR;
    scene.lightSystem.ExportUniforms(useShader, scene.lights);
    scene.skybox.ExportUniformsTo(useShader);

    scene.cameras[scene.activeCam]->updateUniforms(useShader);
    for (const auto& model : scene.models)
        model.Draw(useShader);

    scene.cameras[scene.activeCam]->updateUniforms(IDs.lightSphere);
    scene.lightSystem.DrawLightSpheres(IDs.lightSphere, scene.lights);

    scene.skybox.Draw(IDs.skybox, scene.cameras[scene.activeCam]->getRotationMat());

    postProcessSystem.End(IDs.postProcess, IDs.blur);

    // BindSquare();
    // shadowCasters.DrawDepthDebug(IDs.depthDebug, 0);
    // UnbindSquare();

    scene.imguiFunctions();
}