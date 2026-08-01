#include "main.h"

int main()
{
// Window creation
#pragma region
    Window VIEWPORT;
    Shader defShader("Assets/shaders/default.vert", "Assets/shaders/default.frag");
    defShader.Activate();
    My_ImGui::Init(VIEWPORT.getWindow());

    GLint modelLoc      = glGetUniformLocation(defShader.ID, "translated");
    GLint projLoc       = glGetUniformLocation(defShader.ID, "projection");
    GLint colorLoc      = glGetUniformLocation(defShader.ID, "Color");
    GLint useTextureLoc = glGetUniformLocation(defShader.ID, "useTexture");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 proj  = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    model           = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
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

    Texture squareTexture("Assets/Textures/canion1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    squareTexture.Bind();
    squareTexture.texUnit(defShader, "tex0", 0);

    auto Bind = [](VAO squareVAO, VBO squareVBO, Texture squareTexture)
    {
        squareVAO.Bind();
        squareVBO.Bind();
        squareTexture.Bind();
    };
    auto Unbind = [](VAO squareVAO, VBO squareVBO, Texture squareTexture)
    {
        squareTexture.Unbind();
        squareVBO.Unbind();
        squareVAO.Unbind();
    };

    Unbind(squareVAO, squareVBO, squareTexture);
#pragma endregion

    while (!glfwWindowShouldClose(VIEWPORT.getWindow()))
    {
        VIEWPORT.glClearCurrentColor();
        glClear(GL_COLOR_BUFFER_BIT);
        My_ImGui::ShowDockSpace();

        Bind(squareVAO, squareVBO, squareTexture);
        glUniform1i(useTextureLoc, 1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, indices);
        Unbind(squareVAO, squareVBO, squareTexture);

        My_ImGui::RenderInterfaceInput();
        My_ImGui::RenderDockSpace();
        glfwSwapBuffers(VIEWPORT.getWindow());
        glfwPollEvents();
    }
    defShader.Delete();
    squareTexture.Delete();
    squareVBO.Delete();
    squareVAO.Delete();
#pragma endregion

    return 0;
}
