#include "main.h"
int width = 900;
int height = 900;

GLfloat square[20] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// top-left
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom-right
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f	// top-right
};

GLuint indices[6] = {
	0, 1, 2, //
	1, 2, 3	 //
};

int main()
{
	std::cout << "Hello CMake." << std::endl;

// Window creation
#pragma region
	// Name of the window, width & height of the window, background color RGB
	Window VIEWPORT("Template", width, height, 0.7f, 0.7f, 0.7f);
	VIEWPORT.glfwSetup();
	Shader defShader("Assets/shaders/default.vert", "Assets/shaders/default.frag");
	defShader.Activate();
	My_ImGui::Init(VIEWPORT.getWindow());
#pragma endregion

// Uniform pointers and initial matrix values
#pragma region
	GLint modelLoc = glGetUniformLocation(defShader.ID, "translated");
	GLint projLoc = glGetUniformLocation(defShader.ID, "projection");
	GLint colorLoc = glGetUniformLocation(defShader.ID, "Color");
	GLint useTextureLoc = glGetUniformLocation(defShader.ID, "useTexture");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
#pragma endregion

// Window aspect correction
#pragma region
	struct ResizeData
	{
		GLint projLoc;
		glm::mat4 proj;
		GLuint shaderID;
		int &width;
		int &height;
	};
	ResizeData resizeData{projLoc, proj, defShader.ID, width, height};
	glfwSetWindowUserPointer(VIEWPORT.getWindow(), &resizeData);

	glfwSetFramebufferSizeCallback(VIEWPORT.getWindow(), [](GLFWwindow *win, int w, int h)
								   {
		glViewport(0, 0, w, h);
		auto* data = (ResizeData*)glfwGetWindowUserPointer(win);
		float aspect = (float)w / (float)h;
		data->width = w;
    	data->height = h;
		data->proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
		glUseProgram(data->shaderID);
		glUniformMatrix4fv(data->projLoc, 1, GL_FALSE, glm::value_ptr(data->proj)); });
#pragma endregion

// Square
#pragma region
	VAO squareVAO;
	squareVAO.Bind();
	VBO squareVBO(square, sizeof(square));
	squareVBO.Bind();
	squareVAO.LinkAttrib(squareVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void *)0);
	squareVAO.LinkAttrib(squareVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void *)(sizeof(GLfloat) * 3));

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
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, indices);
		Unbind(squareVAO, squareVBO, squareTexture);

		My_ImGui::RenderInterfaceInput();
		My_ImGui::RenderDockSpace();
		glfwSwapBuffers(VIEWPORT.getWindow());
		glfwPollEvents();
	}

// Cleanup
#pragma region
	My_ImGui::Shutdown();
	defShader.Delete();
	squareTexture.Delete();
	squareVBO.Delete();
	squareVAO.Delete();
#pragma endregion

	return 0;
}
