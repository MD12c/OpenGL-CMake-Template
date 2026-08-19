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

    scene.lightSystem.ExportUniforms(IDs.PBR, scene.lights);

    scene.cameras[scene.activeCam]->updateUniforms(IDs.PBR);
    for (const auto& model : scene.models)
        model.Draw(IDs.PBR, glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                   glm::vec3(1.0f, 1.0f, 1.0f));

    scene.cameras[scene.activeCam]->updateUniforms(IDs.lightSphere);
    scene.lightSystem.DrawLightSpheres(IDs.lightSphere, scene.lights);

    scene.skybox.Draw(IDs.skybox, scene.cameras[scene.activeCam]->getRotationMat());

    postProcessSystem.End(IDs.postProcess, IDs.blur);

    // BindSquare();
    // shadowCasters.DrawDepthDebug(IDs.depthDebug, 0);
    // UnbindSquare();

    scene.imguiFunctions();
}