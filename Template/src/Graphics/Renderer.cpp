#include "Renderer.h"

#include "imgui.h"
#include "Globals.h"
#include "Cameras/Camera.h"
#include "Framebuffers/Square.h"

Renderer::Renderer()
    : antiAlias(),
      finalFrameBuffer(2, true, "PostProcess"),
      bloom(),
      lut(),
      frustum(),
      noTexture(MaterialManager::makeTexture("Assets/Textures/noTexture.png", Texture::TextureType::ALBEDO))
{
    quad = new Square();
    ShaderManager::LoadAllShaders();
    Texture::setNoTextureID(noTexture->ID);
    for (int i = 0; i < RenderFlag::LAST_RENDERFLAG; i++)
        applyFlag(flags[i], i);

    lut.Draw(ShaderID::BRDF_LUT);
}

Renderer::~Renderer()
{
    delete quad;
    ShaderManager::PrintLoadedUniforms();
    ShaderManager::Cleanup();
}

void Renderer::Render(const Scene& scene)
{
    GPUInstrumentationTimer timerGPU("Frame");
    InstrumentationTimer    timerCPU("Frame");
    frustum.setFrustumPlanes(scene.cameras[0]->getCameraMat());
    
    set(DEPTH_b, true);
    set(CULL_b, true);
    set(CULL_FACE_c, GL_BACK);
    glViewport(0, 0, ShadowCaster::SHADOW_MAP_WIDTH, ShadowCaster::SHADOW_MAP_HEIGHT);
    scene.lightResources.ShadowPass(this, scene.models, scene.worldTransform);
    
    set(CULL_b, false);
    set(DEPTH_FUNC_c, GL_LESS);
    set(FRONT_FACE_c, GL_CCW);
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

    set(CULL_b, false);
    set(DEPTH_FUNC_c, GL_LEQUAL);
    scene.skybox.Draw(ShaderID::SKYBOX, scene.cameras[scene.activeCam]->getRotationMat());
    set(DEPTH_FUNC_c, GL_LESS);
    set(CULL_b, true);

    antiAlias.CopyResultsTo(finalFrameBuffer);
    Texture& blurredTexture = bloom.BlurPass(finalFrameBuffer.textures[1], ShaderID::BLUR, 3);

    ShaderManager::Activate(ShaderID::POSTPROCESS);
    glUniform1f(ShaderManager::getLoc(ShaderID::POSTPROCESS, "gamma"), gamma);
    finalFrameBuffer.textures[0].texUnit(ShaderID::POSTPROCESS, "tex0");
    blurredTexture.texUnit(ShaderID::POSTPROCESS, "tex1");
    
    BindFramebuffer(0);
    glViewport(0, 0, width, height);
    set(DEPTH_b, false);
    quad->DrawSquare();
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    set(DEPTH_b, true);
    Texture::UnbindAll();
    CubeTexture::UnbindAll();
    // std::cout << glGetError() << std::endl;
    scene.imguiFunctions();
}

void Renderer::set(RenderFlag f, GLenum value)
{
    if (flags[f] == value)
        return;
    flags[f] = value;

    switch (f)
    {
        case DEPTH_b:
            applyFlag(value, GL_DEPTH_TEST);
            break;
        case STENCIL_b:
            applyFlag(value, GL_STENCIL_TEST);
            break;
        case COLOR_b:
            applyFlag(value, GL_COLOR_LOGIC_OP);
            break;
        case CULL_b:
            applyFlag(value, GL_CULL_FACE);
            break;
        case CULL_FACE_c:
            glCullFace(value);
            break;
        case FRONT_FACE_c:
            glFrontFace(value);
            break;
        case DEPTH_FUNC_c:
            glDepthFunc(value);
            break;
        case MULTISAMPLE_b:
            applyFlag(value, GL_MULTISAMPLE);
            break;
        default:
            break;
    }
}