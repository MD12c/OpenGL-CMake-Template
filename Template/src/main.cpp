#include "main.h"

int main()
{
// Window creation
#pragma region
    Window VIEWPORT;
    My_ImGui::Init(VIEWPORT.getWindow());
    // Camera2D camera(VIEWPORT.getWindow());
    Orbit camera(glm::vec3(1.0f, 0.0f, 0.0f));
    glfwPtr.window = &VIEWPORT;
    glfwPtr.camera = &camera;
#pragma endregion

// Shaders
#pragma region
    Shader modelShader("Assets/shaders/model.vert", "Assets/shaders/model.frag", "Assets/shaders/model.geom");
    modelShader.Activate();
    GLuint cameraMatrixModLoc   = glGetUniformLocation(modelShader.ID, "cameraMatrix");
    GLuint cameraPositionModLoc = glGetUniformLocation(modelShader.ID, "camPos");
    glUniform4f(glGetUniformLocation(modelShader.ID, "lightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(modelShader.ID, "lightPos"), 1.0f, 1.0f, 1.0f);

    Shader defShader("Assets/shaders/default.vert", "Assets/shaders/default.frag");
    defShader.Activate();
    GLint  cameraMatrixdefLoc   = glGetUniformLocation(defShader.ID, "cameraMatrix");
    GLuint cameraPositionDefLoc = glGetUniformLocation(modelShader.ID, "camPos");
    GLint  colorLoc             = glGetUniformLocation(defShader.ID, "Color");
    GLint  useTextureLoc        = glGetUniformLocation(defShader.ID, "useTexture");

    glUniformMatrix4fv(cameraMatrixdefLoc, 1, GL_FALSE, glm::value_ptr(camera.cameraMatrix));
    glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
#pragma endregion

// Square
#pragma region
    VAO squareVAO;
    squareVAO.Bind();
    VBO squareVBO(square, sizeof(square), GL_STATIC_DRAW);
    squareVBO.Bind();
    squareVAO.LinkAttrib(squareVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    squareVAO.LinkAttrib(squareVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(sizeof(GLfloat) * 3));

    Texture squareTexture("Assets/Textures/canion1.png", "GL_TEXTURE_2D", GL_TEXTURE0);
    squareTexture.Bind();
    squareTexture.texUnit(defShader, "tex0", 0);

    auto Bind = [&]()
    {
        squareVAO.Bind();
        squareVBO.Bind();
        squareTexture.Bind();
    };
    auto Unbind = [&]()
    {
        squareTexture.Unbind();
        squareVBO.Unbind();
        squareVAO.Unbind();
    };

    Unbind();
#pragma endregion

// Models
#pragma region
    Model model("Assets/Models/crow.obj");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
#pragma endregion

    double timePrev = 0;
    double timeCrnt = 0;
    double timeDiff;

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
        VIEWPORT.updateFPS();

        VIEWPORT.glClearCurrentColor();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        My_ImGui::ShowDockSpace();

        camera.updateUniforms(VIEWPORT.getWindow(), cameraMatrixdefLoc, cameraPositionDefLoc);
        Bind();
        glUniform1i(useTextureLoc, 1);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(GLuint), GL_UNSIGNED_INT, squareIndices);
        Unbind();

        modelShader.Activate();
        camera.updateUniforms(VIEWPORT.getWindow(), cameraMatrixModLoc, cameraPositionModLoc);
        model.Draw(modelShader);

        My_ImGui::RenderInterfaceInput();
        My_ImGui::RenderDockSpace();
        glfwSwapBuffers(VIEWPORT.getWindow());
        glfwPollEvents();
    }
    // defShader.Delete();
    modelShader.Delete();
    // squareTexture.Delete();
    // squareVBO.Delete();
    // squareVAO.Delete();
#pragma endregion

    return 0;
}
