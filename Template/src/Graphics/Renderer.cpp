#include "Renderer.h"

#include "imgui.h"
#include "LightSystem.h"

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
    timeCrnt = glfwGetTime();
    timeDiff = timeCrnt - timePrev;
    if (timeDiff >= 1.0 / 60.0)
    {
        timePrev = timeCrnt;
        if (!ImGui::GetIO().WantCaptureMouse)
            scene.cameras[scene.activeCam]->Inputs(scene.glfwWindowPtr);
    }

    for (const auto& model : scene.models)
        scene.lightSystem.ShadowPass(model);

    antiAlias.Activate();

    scene.lightSystem.ExportUniforms(ShaderManager::IDs.model);

    scene.cameras[scene.activeCam]->updateUniforms(ShaderManager::IDs.model);
    for (const auto& model : scene.models)
        model.Draw(ShaderManager::IDs.model);
    scene.lightSystem.RenderLightModels(ShaderManager::IDs.model);

    scene.skybox.Draw(ShaderManager::IDs.skybox, scene.cameras[scene.activeCam]->getRotationMat());

    antiAlias.CopyResultsTo(postProcess.ID);
    postProcess.Draw(ShaderManager::IDs.postProcess);

    // BindSquare();
    // shadowCasters.DrawDepthDebug(IDs.depthDebug, 0);
    // UnbindSquare();

    scene.imguiFunctions();
}