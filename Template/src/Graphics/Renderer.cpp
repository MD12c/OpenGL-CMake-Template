#include "Renderer.h"

#include "imgui.h"
#include "Lighting\LightSystem.h"

using namespace ShaderManager;

Renderer::Renderer()
    : antiAlias(),
      postProcess()
{
}

Renderer::~Renderer()
{
}

void Renderer::Render(const Scene& scene)
{
    scene.lightSystem.ShadowPass(scene.models, scene.lights);

    antiAlias.Activate();

    scene.lightSystem.ExportUniforms(IDs.model, scene.lights);

    scene.cameras[scene.activeCam]->updateUniforms(IDs.model);

    for (const auto& model : scene.models)
        model.Draw(IDs.model);

    scene.lightSystem.DrawLightSpheres(IDs.model, scene.lights);

    scene.skybox.Draw(IDs.skybox, scene.cameras[scene.activeCam]->getRotationMat());

    antiAlias.CopyResultsTo(postProcess.ID);
    postProcess.Draw(IDs.postProcess);

    // BindSquare();
    // shadowCasters.DrawDepthDebug(IDs.depthDebug, 0);
    // UnbindSquare();

    scene.imguiFunctions();
}