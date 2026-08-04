#include "main.h"

int main()
{
// Window creation
#pragma region
    Window VIEWPORT;
    My_ImGui::Init(VIEWPORT.getWindow());

    Camera2D camera(VIEWPORT.getWindow());
    //Orbit camera(VIEWPORT.getWindow(), glm::vec3(1.0f, 0.0f, 0.0f));
    //CameraFly camera(VIEWPORT.getWindow(), glm::vec3(0.0f), 45, 0.1f, 10000.0f);

    Framebuffer framebuffer;
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
            { "lightPos", ShaderManager::GetUniformLoc(modelShaderName, "lightPos") }
        };
        ShaderManager::AddUniforms(modelShaderName, modelShaderUniforms);
    }
    ShaderManager::Activate(modelShaderName);
    glUniform4f(ShaderManager::GetUniformLoc(modelShaderName, "lightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform3f(ShaderManager::GetUniformLoc(modelShaderName, "lightPos"), -4.0f, 17.0f, 13.0f);

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

    // Framebuffer
        std::string framebufferShaderName = ShaderManager::Load("framebufferShader", "Assets/shaders/framebuffer.vert", "Assets/shaders/framebuffer.frag");
    {
        std::unordered_map<std::string, GLint> framebufferShaderUniforms = {
            { "screenTexture", ShaderManager::GetUniformLoc(framebufferShaderName, "screenTexture") }
        };
        ShaderManager::AddUniforms(framebufferShaderName, framebufferShaderUniforms);
    }
    ShaderManager::Activate(framebufferShaderName);
    glUniform1i(ShaderManager::GetUniformLoc(framebufferShaderName, "screenTexture"), 0);

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

        framebuffer.Activate();

        ShaderManager::Activate(defShaderName);
        camera.updateUniforms(defShaderName);
        BindSquare();
        glUniform1i(ShaderManager::GetUniformLoc(defShaderName, "useTexture"), true);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(GLuint), GL_UNSIGNED_INT, squareIndices);
        UnbindSquare();

        camera.updateUniforms(modelShaderName);
        model.Draw(modelShaderName);

        skybox.Draw(skyboxShaderName, camera.getRotationMat());

        framebuffer.Draw(framebufferShaderName);

        My_ImGui::RenderOverlay(camera.position.x, camera.position.y, camera.position.z);
        My_ImGui::RenderInterfaceInput();
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
