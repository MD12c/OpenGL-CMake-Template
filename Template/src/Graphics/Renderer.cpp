#include "Renderer.h"

#include "imgui.h"
#include "Globals.h"
#include "Cameras/Camera.h"
#include "Lighting\LightSystem.h"

Renderer::Renderer()
    : postProcessSystem(),
      frustum(),
      noTexture(MaterialManager::makeTexture("Assets/Textures/noTexture.png", Texture::TextureType::ALBEDO))
{
    ShaderManager::LoadAllShaders();
    Texture::setNoTextureID(noTexture->ID);
    postProcessSystem.lut.Draw(ShaderID::BRDF_LUT);
}

Renderer::~Renderer()
{
    ShaderManager::PrintLoadedUniforms();
    ShaderManager::Cleanup();
}

void Renderer::Render(const Scene& scene)
{
    GPUInstrumentationTimer timer("Frame");
    frustum.setFrustumPlanes(scene.cameras[0]->getCameraMat());
    
    scene.lightSystem.ShadowPass(scene.models, scene.lights, scene.worldTransform);

    postProcessSystem.Begin();

    auto useShader = ShaderID::PBR;
    scene.lightSystem.ExportUniforms(useShader, scene.lights);
    scene.skybox.ExportUniformsTo(useShader);
    postProcessSystem.lut.ExportUniformsTo(useShader);

    scene.cameras[scene.activeCam]->updateUniforms(useShader);
    for (const auto& model : scene.models)
        model.Draw(useShader, scene.worldTransform, &frustum);

    scene.cameras[scene.activeCam]->updateUniforms(ShaderID::LIGHT_SPHERE);
    scene.lightSystem.DrawLightSpheres(ShaderID::LIGHT_SPHERE, scene.lights);

    scene.skybox.Draw(ShaderID::SKYBOX, scene.cameras[scene.activeCam]->getRotationMat());

    postProcessSystem.End();

    scene.imguiFunctions();
}