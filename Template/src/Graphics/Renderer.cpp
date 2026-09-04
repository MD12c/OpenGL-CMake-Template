#include "Renderer.h"

#include "Globals.h"
#include "Cameras/Camera.h"
#include "Framebuffers/Square.h"
#include "Shaders/Shader.h"
#include "Materials/MaterialManager.h"
#include "Models/BasicShapes.h"
#include "Window.h"

using RF = RenderFeature;
using DF = DepthFunc;
using CM = CullMode;

Renderer::Renderer(GLfloat gamma)
    : antiAlias(),
      finalFrameBuffer(2, false, Window::width, Window::height, "PostProcess"),
      bloom(Window::width, Window::height),
      lut(Window::width, Window::height),
      frustum(),
      gamma(gamma),
      noTexture(MaterialManager::makeTexture("Assets/Textures/noTexture.png", Texture::TextureType::ALBEDO))
{
    basicShapes = new BasicShapes();
    Shader::LoadAllShaders();
    Texture::setNoTextureID(noTexture->ID);
    glFrontFace(GL_CCW);
    glViewport(0, 0, Window::width, Window::height);
    lut.Draw(ShaderID::BRDF_LUT);
}

Renderer::~Renderer()
{
    delete basicShapes;
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
    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    scene.lightResources.ShadowPass(this, scene.models, scene.worldTransform);

    set(RF::CULL, false);
    setDepthFunc(DF::LESS);
    glViewport(0, 0, Window::width, Window::height);

    BindFramebuffer(antiAlias.ID);
    antiAlias.MSAAbufferRBO.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    set(RF::CULL, true);
    auto useShader = ShaderID::PBR;
    scene.lightResources.ExportUniformsTo(useShader);
    scene.skybox.ExportUniformsTo(useShader);
    lut.ExportUniformsTo(useShader);
    scene.cameras[scene.activeCam]->updateUniforms(useShader);
    for (const auto& model : scene.models)
        model.Draw(useShader, scene.worldTransform, &frustum);

    scene.cameras[scene.activeCam]->updateUniforms(ShaderID::LIGHT_SPHERE);
    const auto& Lres = scene.lightResources;
    for (size_t i = 0; i < Lres.pointLights.size(); i++)
    {
        set(RF::CULL, true);
        Lres.DrawLightSpheres(ShaderID::LIGHT_SPHERE,
                              Lres.pointLights[i].getPosition(),
                              Lres.pointLights[i].getColor());
    }
    for (size_t i = 0; i < Lres.directionLights.size(); i++)
    {
        set(RF::CULL, false);
        Lres.DrawLightPlanes(ShaderID::LIGHT_SPHERE,
                             Lres.directionLights[i].getPosition(),
                             Lres.directionLights[i].getDirection(),
                             Lres.directionLights[i].getColor());
    }

    set(RF::CULL, false);
    setDepthFunc(DF::LEQUAL);
    scene.skybox.Draw(ShaderID::SKYBOX, scene.cameras[scene.activeCam]->getRotationMat());
    setDepthFunc(DF::LESS);
    set(RF::CULL, true);

    antiAlias.CopyResultsTo(finalFrameBuffer);
    glViewport(0, 0, Window::width, Window::height);
    Texture& blurredTexture = bloom.BlurPass(finalFrameBuffer.textures[1], ShaderID::BLUR, 3);

    Shader::Activate(ShaderID::POSTPROCESS);
    glUniform1f(Shader::getLoc(ShaderID::POSTPROCESS, "gamma"), gamma);
    finalFrameBuffer.textures[0].texUnit(ShaderID::POSTPROCESS, "tex0");
    blurredTexture.texUnit(ShaderID::POSTPROCESS, "tex1");

    BindFramebuffer(0);
    glViewport(0, 0, Window::width, Window::height);
    set(RF::DEPTH, false);
    basicShapes->plane.DrawSimple(ShaderID::POSTPROCESS);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    set(RF::DEPTH, true);
    Texture::UnbindAll();
    CubeTexture::UnbindAll();

    // std::cout << glGetError() << "\n";

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