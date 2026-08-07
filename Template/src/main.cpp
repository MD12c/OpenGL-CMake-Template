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
    // Model
    std::string modelShaderName = ShaderManager::Load("modelShader", "Assets/shaders/model.vert", "Assets/shaders/model.frag", "Assets/shaders/model.geom");
    {
        std::unordered_map<std::string, GLint> modelShaderUniforms = {
            { "proj", ShaderManager::GetUniformLoc(modelShaderName, "proj") },
            { "view", ShaderManager::GetUniformLoc(modelShaderName, "view") },
            { "model", ShaderManager::GetUniformLoc(modelShaderName, "model") },
            { "camPos", ShaderManager::GetUniformLoc(modelShaderName, "camPos") },
            { "lightColor", ShaderManager::GetUniformLoc(modelShaderName, "lightColor") },
            { "lightPos", ShaderManager::GetUniformLoc(modelShaderName, "lightPos") },
            { "lightDirection", ShaderManager::GetUniformLoc(modelShaderName, "lightDirection") },
            { "shadowMap", ShaderManager::GetUniformLoc(modelShaderName, "shadowMap") },
            { "shadowMapMatrix", ShaderManager::GetUniformLoc(modelShaderName, "shadowMapMatrix") },
            { "shadowCubeMap", ShaderManager::GetUniformLoc(modelShaderName, "shadowCubeMap") },
            { "farPlane", ShaderManager::GetUniformLoc(modelShaderName, "farPlane") },
        };
        ShaderManager::AddUniforms(modelShaderName, modelShaderUniforms);
    }
    ShaderManager::Activate(modelShaderName);
    // glm::vec3 lightPos = glm::vec3(0.0f, 30.0f, 0.0f);
    glm::vec3 lightPos = glm::vec3(-4.5f, 17.0f, 3.0f);
    glUniform4f(ShaderManager::GetUniformLoc(modelShaderName, "lightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform3fv(ShaderManager::GetUniformLoc(modelShaderName, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3f(ShaderManager::GetUniformLoc(modelShaderName, "lightDirection"), 0.0f, -1.0f, 0.0f);

    // SkyBox
    std::string skyboxShaderName = ShaderManager::Load("skyboxShader", "Assets/shaders/skybox.vert", "Assets/shaders/skybox.frag");
    {
        std::unordered_map<std::string, GLint> skyboxShaderUniforms = {
            { "cameraMatrix", ShaderManager::GetUniformLoc(skyboxShaderName, "cameraMatrix") },
            { "skybox", ShaderManager::GetUniformLoc(skyboxShaderName, "skybox") }
        };
        ShaderManager::AddUniforms(skyboxShaderName, skyboxShaderUniforms);
    }
    ShaderManager::Activate(skyboxShaderName);
    glUniform1i(ShaderManager::GetUniformLoc(skyboxShaderName, "skybox"), 0);

    // postProcess
    std::string postProcessShaderName = ShaderManager::Load("postProcessShader", "Assets/shaders/postProcess.vert", "Assets/shaders/postProcess.frag");
    {
        std::unordered_map<std::string, GLint> postProcessShaderUniforms = {
            { "screenTexture", ShaderManager::GetUniformLoc(postProcessShaderName, "screenTexture") },
            { "gamma", ShaderManager::GetUniformLoc(postProcessShaderName, "gamma") }
        };
        ShaderManager::AddUniforms(postProcessShaderName, postProcessShaderUniforms);
    }
    ShaderManager::Activate(postProcessShaderName);
    glUniform1i(ShaderManager::GetUniformLoc(postProcessShaderName, "screenTexture"), 0);
    glUniform1f(ShaderManager::GetUniformLoc(postProcessShaderName, "gamma"), gamma);

    // shadowMap2D
    std::string shadowMap2DShaderName = ShaderManager::Load("shadowMap2DShader", "Assets/shaders/shadowMap2D.vert", "Assets/shaders/shadowMap2D.frag");
    {
        std::unordered_map<std::string, GLint> shadowMap2DShaderUniforms = {
            { "proj", ShaderManager::GetUniformLoc(shadowMap2DShaderName, "proj") },
            { "model", ShaderManager::GetUniformLoc(shadowMap2DShaderName, "model") },
            { "view", ShaderManager::GetUniformLoc(shadowMap2DShaderName, "view") }
        };
        ShaderManager::AddUniforms(shadowMap2DShaderName, shadowMap2DShaderUniforms);
    }

    // shadowMapCube
    std::string shadowMapCubeShaderName = ShaderManager::Load("shadowMapCubeShader", "Assets/shaders/shadowMapCube.vert", "Assets/shaders/shadowMapCube.frag", "Assets/shaders/shadowMapCube.geom");
    {
        std::unordered_map<std::string, GLint> shadowMapCubeShaderUniforms = {
            { "model", ShaderManager::GetUniformLoc(shadowMapCubeShaderName, "model") },
            { "lightPos", ShaderManager::GetUniformLoc(shadowMapCubeShaderName, "lightPos") },
            { "farPlane", ShaderManager::GetUniformLoc(shadowMapCubeShaderName, "farPlane") }
        };
        ShaderManager::AddUniforms(shadowMapCubeShaderName, shadowMapCubeShaderUniforms);
        for (int i = 0; i < 6; i++)
            ShaderManager::AddUniform(shadowMapCubeShaderName,
                                      "shadowMatrices[" + std::to_string(i) + "]",
                                      ShaderManager::GetUniformLoc(shadowMapCubeShaderName, "shadowMatrices[" + std::to_string(i) + "]"));
    }

    // Debug
    std::string depthDebugShaderName = ShaderManager::Load("depthDebugShader", "Assets/shaders/debug.vert", "Assets/shaders/debug.frag");
    {
        std::unordered_map<std::string, GLint> depthDebugUniforms = {
            { "depthMap", ShaderManager::GetUniformLoc(depthDebugShaderName, "depthMap") }
        };
        ShaderManager::AddUniforms(depthDebugShaderName, depthDebugUniforms);
    }
    ShaderManager::Activate(depthDebugShaderName);
    glUniform1i(ShaderManager::GetUniformLoc(depthDebugShaderName, "depthMap"), 0);

    // DebugCube
    std::string depthDebugCubeShaderName = ShaderManager::Load("depthDebugCubeShader", "Assets/shaders/debug.vert", "Assets/shaders/debugCube.frag");
    {
        std::unordered_map<std::string, GLint> depthDebugCubeUniforms = {
            { "depthCubeMap", ShaderManager::GetUniformLoc(depthDebugCubeShaderName, "depthCubeMap") },
            { "faceForward", ShaderManager::GetUniformLoc(depthDebugCubeShaderName, "faceForward") },
            { "faceRight", ShaderManager::GetUniformLoc(depthDebugCubeShaderName, "faceRight") },
            { "faceUp", ShaderManager::GetUniformLoc(depthDebugCubeShaderName, "faceUp") },
        };
        ShaderManager::AddUniforms(depthDebugCubeShaderName, depthDebugCubeUniforms);
    }
    ShaderManager::Activate(depthDebugCubeShaderName);

    // Default
    std::string defShaderName = ShaderManager::Load("defShader", "Assets/shaders/default.vert", "Assets/shaders/default.frag");
    {
        std::unordered_map<std::string, GLint> defShaderUniforms = {
            { "proj", ShaderManager::GetUniformLoc(defShaderName, "proj") },
            { "view", ShaderManager::GetUniformLoc(defShaderName, "view") },
            { "camPos", ShaderManager::GetUniformLoc(defShaderName, "camPos") },
            { "Color", ShaderManager::GetUniformLoc(defShaderName, "Color") },
            { "useTexture", ShaderManager::GetUniformLoc(defShaderName, "useTexture") },
            { "tex0", ShaderManager::GetUniformLoc(defShaderName, "tex0") }
        };
        ShaderManager::AddUniforms(defShaderName, defShaderUniforms);
    }
    ShaderManager::Activate(defShaderName);
    glUniform3fv(ShaderManager::GetUniformLoc(defShaderName, "Color"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));

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
    squareTexture.texUnit(defShaderName, "tex0");

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
    // ShadowMap2D shadowMap(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), -35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 400.0f);// -100.0f, 100.0f, -100.0f, 100.0f
    // ShadowMap2D shadowMap(lightPos, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f, 0.1f, 400.0f);
    ShadowMapCube shadowMap(lightPos, 0.1f, 400.0f);
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
        // VIEWPORT.updateFPS(); // Can be overlayed

        VIEWPORT.glClearCurrentColor();
        My_ImGui::ShowDockSpace();

        shadowMap.BeginDepthPass(shadowMapCubeShaderName);
        if (glfwGetKey(VIEWPORT.getWindow(), GLFW_KEY_B) == GLFW_PRESS)
            shadowMap.setView(camera.position);  //, camera.Orientation);
        model.Draw(shadowMapCubeShaderName);

        antiAlias.Activate();

        // ShaderManager::Activate(defShaderName);
        // camera.updateUniforms(defShaderName);
        // BindSquare();
        // glUniform1i(ShaderManager::GetUniformLoc(defShaderName, "useTexture"), true);
        // // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(GLuint), GL_UNSIGNED_INT, squareIndices);
        // UnbindSquare();

        shadowMap.ExportUniformsTo(modelShaderName, 2);
        camera.updateUniforms(modelShaderName);
        if (glfwGetKey(VIEWPORT.getWindow(), GLFW_KEY_B) == GLFW_PRESS)
        {
            glUniform3fv(ShaderManager::GetUniformLoc(modelShaderName, "lightPos"), 1, glm::value_ptr(camera.position));
            glUniform3fv(ShaderManager::GetUniformLoc(modelShaderName, "lightDirection"), 1, glm::value_ptr(camera.Orientation));
        }
        model.Draw(modelShaderName);

        skybox.Draw(skyboxShaderName, camera.getRotationMat());

        antiAlias.CopyResultsTo(postProcess.ID);
        postProcess.Draw(postProcessShaderName);

        // BindSquare();
        // shadowMap.DrawDepthDebug(depthDebugCubeShaderName, 0);
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
