#include "Renderer.h"

#include "Globals.h"
#include "Cameras/Camera.h"
#include "Framebuffers/Square.h"
#include "Shaders/Shader.h"
#include "Materials/MaterialManager.h"

using RF = RenderFeature;
using DF = DepthFunc;
using CM = CullMode;

Renderer::Renderer()
    : antiAlias(),
      finalFrameBuffer(2, true, "PostProcess"),
      bloom(),
      lut(),
      frustum(),
      noTexture(MaterialManager::makeTexture("Assets/Textures/noTexture.png", Texture::TextureType::ALBEDO))
{
    quad = new Square();
    Shader::LoadAllShaders();
    Texture::setNoTextureID(noTexture->ID);
    glFrontFace(GL_CCW);
    lut.Draw(ShaderID::BRDF_LUT);
}

Renderer::~Renderer()
{
    delete quad;
    // Shader::PrintLoadedUniforms();
    Shader::Cleanup();
}

void Renderer::Render(const Scene& scene)
{
    GPUInstrumentationTimer timerGPU("Frame");
    InstrumentationTimer    timerCPU("Frame");
    frustum.setFrustumPlanes(scene.cameras[0]->getCameraMat());
    
    set(RF::DEPTH, true);
    set(RF::CULL, true);
    setCullMode(CM::BACK);
    glViewport(0, 0, ShadowCaster::SHADOW_MAP_WIDTH, ShadowCaster::SHADOW_MAP_HEIGHT);
    scene.lightResources.ShadowPass(this, scene.models, scene.worldTransform);

    set(RF::CULL, false);
    setDepthFunc(DF::LESS);
    glViewport(0, 0, width, height);

    BindFramebuffer(antiAlias.ID);
    antiAlias.MSAAbufferRBO.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    auto useShader = ShaderID::PBR;
    scene.lightResources.ExportUniformsTo(useShader);
    scene.skybox.ExportUniformsTo(useShader);
    lut.ExportUniformsTo(useShader);
    scene.cameras[scene.activeCam]->updateUniforms(useShader);
    for (const auto& model : scene.models)
        model.Draw(useShader, scene.worldTransform, &frustum);

    scene.cameras[scene.activeCam]->updateUniforms(ShaderID::LIGHT_SPHERE);
    scene.lightResources.DrawLightSpheres(ShaderID::LIGHT_SPHERE);

    set(RF::CULL, false);
    setDepthFunc(DF::LEQUAL);
    scene.skybox.Draw(ShaderID::SKYBOX, scene.cameras[scene.activeCam]->getRotationMat());
    setDepthFunc(DF::LESS);
    set(RF::CULL, true);

    antiAlias.CopyResultsTo(finalFrameBuffer);
    Texture& blurredTexture = bloom.BlurPass(finalFrameBuffer.textures[1], ShaderID::BLUR, 3);

    Shader::Activate(ShaderID::POSTPROCESS);
    glUniform1f(Shader::getLoc(ShaderID::POSTPROCESS, "gamma"), gamma);
    finalFrameBuffer.textures[0].texUnit(ShaderID::POSTPROCESS, "tex0");
    blurredTexture.texUnit(ShaderID::POSTPROCESS, "tex1");

    BindFramebuffer(0);
    glViewport(0, 0, width, height);
    set(RF::DEPTH, false);
    quad->DrawSquare();
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    set(RF::DEPTH, true);
    Texture::UnbindAll();
    CubeTexture::UnbindAll();

    scene.imguiFunctions();
}

void Renderer::set(RenderFeature feature, bool enable)
{
    size_t index = static_cast<size_t>(feature);
    if (enabledFeatures[index] == enable) return;
    enabledFeatures[index] = enable;

    switch (feature)
    {
        case RenderFeature::DEPTH:
            enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
            break;
        case RenderFeature::STENCIL:
            enable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
            break;
        case RenderFeature::CULL:
            enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
            break;
        case RenderFeature::MULTISAMPLE:
            enable ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
            break;
    }
}

void Renderer::setDepthFunc(DepthFunc func)
{
    if (currentDepthFunc == func) return;
    currentDepthFunc = func;
    glDepthFunc(static_cast<GLenum>(func));
}

void Renderer::setCullMode(CullMode mode)
{
    if (currentCullMode == mode) return;
    currentCullMode = mode;
    glCullFace(static_cast<GLenum>(mode));
}