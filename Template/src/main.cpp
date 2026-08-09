#include "main.h"

int main()
{
// Window creation
#pragma region
    Window VIEWPORT;
    Renderer renderer;
    My_ImGui::Init(VIEWPORT.getWindow());
    ShaderManager::LoadAllShaders();

    Scene scene(VIEWPORT.getWindow());
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

    while (!glfwWindowShouldClose(VIEWPORT.getWindow()))
    {
        // VIEWPORT.updateFPS(); // Can be overlayed in imgui
        VIEWPORT.glClearCurrentColor();
        My_ImGui::ShowDockSpace();

        renderer.Render(scene);
        
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
