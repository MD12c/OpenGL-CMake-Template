#include "main.h"

int main()
{
// Window creation
#pragma region
    Window VIEWPORT;
    My_ImGui::Init(VIEWPORT.getWindow());

    // Camera2D camera(VIEWPORT.getWindow());
    // CameraOrbit camera(VIEWPORT.getWindow());
    CameraFly camera(VIEWPORT.getWindow(), 45, 0.1f, 10000.0f);

    MSAAbuffer  antiAlias;
    Framebuffer postProcess;
#pragma endregion

// Shaders
#pragma region
    ShaderManager::LoadAllShaders();
#pragma endregion

// Square
#pragma region
    VAO squareVAO;
    squareVAO.Bind();
    VBO squareVBO(square, sizeof(square), GL_STATIC_DRAW);
    squareVBO.Bind();
    squareVAO.LinkAttrib(squareVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    squareVAO.LinkAttrib(squareVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(sizeof(GLfloat) * 3));

    Texture squareTexture("Assets/Textures/canion1.png", "diffuse", GL_TEXTURE0);
    squareTexture.Bind();
    squareTexture.texUnit(ShaderManager::IDs.def, "tex0");

    auto BindSquare = [&]()
    {
        squareVAO.Bind();
        squareVBO.Bind();
        squareTexture.Bind();
    };
    auto UnbindSquare = [&]()
    {
        squareTexture.Unbind();
        squareVBO.Unbind();
        squareVAO.Unbind();
    };

    UnbindSquare();
#pragma endregion

// Models
#pragma region
    Model model("Assets/Models/crow.obj");
    glfwPtr.camera = &camera;
    Skybox skybox;

    glm::vec3                                  lightColor = glm::vec3(1.0f);
    std::vector<std::unique_ptr<ShadowCaster>> shadowCasters;
    // ShadowMap2D   shadowMapDir(lightPosition, lightColor, glm::vec3(0.0f, 0.0f, 0.0f), -35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 400.0f);
    // ShadowMap2D   shadowMapSpot(lightPosition, lightColor, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f, 0.1f, 0.90f, 0.95f, 400.0f);
    // ShadowMapCube shadowMapPoint(lightPosition, lightColor, 0.1f, 400.0f);
    shadowCasters.emplace_back(std::make_unique<ShadowMap2D>(lightPosition, lightColor, glm::vec3(0.0f, 0.0f, 0.0f), -35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 400.0f));
    shadowCasters.emplace_back(std::make_unique<ShadowMap2D>(lightPosition, lightColor, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f, 0.1f, 0.90f, 0.95f, 400.0f));
    shadowCasters.emplace_back(std::make_unique<ShadowMapCube>(lightPosition, lightColor, 0.1f, 400.0f));
#pragma endregion

    double timePrev = 0;
    double timeCrnt = 0;
    double timeDiff;
    glfwPtr.camera = &camera;

    while (!glfwWindowShouldClose(VIEWPORT.getWindow()))
    {
        timeCrnt = glfwGetTime();
        timeDiff = timeCrnt - timePrev;
        if (timeDiff >= 1.0 / 60.0)
        {
            timePrev = timeCrnt;
            if (!ImGui::GetIO().WantCaptureMouse)
                camera.Inputs(VIEWPORT.getWindow());
        }
        // VIEWPORT.updateFPS(); // Can be overlayed in imgui

        VIEWPORT.glClearCurrentColor();
        My_ImGui::ShowDockSpace();

        for (auto& caster : shadowCasters)
        {
            caster->BeginDepthPass(caster->getShaderIDfromType(caster->getType()));
            model.Draw(caster->getShaderIDfromType(caster->getType()));
            caster->EndDepthPass();
        }

        // shadowMap.BeginDepthPass(ShaderManager::IDs.shadowMapCube);
        // if (glfwGetKey(VIEWPORT.getWindow(), GLFW_KEY_B) == GLFW_PRESS)
        //     shadowMap.setView(camera.position, camera.Orientation);
        // model.Draw(ShaderManager::IDs.shadowMapCube);

        antiAlias.Activate();

        GLuint nextSlot = 2;
        int    dirIdx = 0, spotIdx = 0, pointIdx = 0;
        for (auto& caster : shadowCasters)
        {
            switch (caster->getType())
            {
                case ShadowCaster::LightType::DIRECTION:
                    caster->ExportUniformsTo(ShaderManager::IDs.model, nextSlot++, dirIdx);
                    dirIdx++;
                    break;
                case ShadowCaster::LightType::SPOT:
                    caster->ExportUniformsTo(ShaderManager::IDs.model, nextSlot++, spotIdx);
                    spotIdx++;
                    break;
                case ShadowCaster::LightType::POINT:
                    caster->ExportUniformsTo(ShaderManager::IDs.model, nextSlot++, pointIdx);
                    pointIdx++;
                    break;
            }
        }
        glUniform1i(ShaderManager::getLoc(ShaderManager::IDs.model, "numDirLights"), dirIdx);
        glUniform1i(ShaderManager::getLoc(ShaderManager::IDs.model, "numSpotLights"), spotIdx);
        glUniform1i(ShaderManager::getLoc(ShaderManager::IDs.model, "numPointLights"), pointIdx);

        // shadowMap.ExportUniformsTo(ShaderManager::IDs.model, 2);
        // if (glfwGetKey(VIEWPORT.getWindow(), GLFW_KEY_B) == GLFW_PRESS)
        // {
        //     glUniform3fv(ShaderManager::getLoc(ShaderManager::IDs.model, "lightPos"), 1, glm::value_ptr(camera.position));
        //     glUniform3fv(ShaderManager::getLoc(ShaderManager::IDs.model, "lightDirection"), 1, glm::value_ptr(camera.Orientation));
        // }
        camera.updateUniforms(ShaderManager::IDs.model);
        model.Draw(ShaderManager::IDs.model);

        skybox.Draw(ShaderManager::IDs.skybox, camera.getRotationMat());

        antiAlias.CopyResultsTo(postProcess.ID);
        postProcess.Draw(ShaderManager::IDs.postProcess);

        // BindSquare();
        // shadowCasters.DrawDepthDebug(IDs.depthDebug, 0);
        // UnbindSquare();

        My_ImGui::RenderOverlay(camera.position.x, camera.position.y, camera.position.z);
        // My_ImGui::RenderInterfaceInput();
        My_ImGui::RenderDockSpace();
        glfwSwapBuffers(VIEWPORT.getWindow());
        glfwPollEvents();
    }
    squareTexture.Delete();
    squareVBO.Delete();
    squareVAO.Delete();
#pragma endregion

    return 0;
}
