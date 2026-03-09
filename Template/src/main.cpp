#include "main.h"
constexpr unsigned int width = 900;
constexpr unsigned int height = 900;

GLfloat triangle[9] = {
	-1.0f, -1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f
};


int main()
{
	std::cout << "Hello CMake." << std::endl;

	// Name of the window, width & height of the window, background color RGB
	Window VIEWPORT("Template", width, height, 0.7f, 0.7f, 0.7f);
	VIEWPORT.glfwSetup();
	Shader defShader("Assets/shaders/default.vert", "Assets/shaders/default.frag");
	defShader.Activate();

	VAO triangleVAO;
	triangleVAO.Bind();
	VBO triangleVBO(triangle, sizeof(triangle));
	triangleVBO.Bind();
	triangleVAO.LinkAttrib(triangleVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	Texture triangleTexture("Assets/Textures/canion1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	triangleTexture.Bind();
	triangleTexture.texUnit(defShader, "tex0", 0);

	GLint modelLoc = glGetUniformLocation(defShader.ID, "translated");
	GLint colorLoc = glGetUniformLocation(defShader.ID, "Color");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

	while (!glfwWindowShouldClose(VIEWPORT.getWindow())) {
		VIEWPORT.glClearCurrentColor();
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(VIEWPORT.getWindow());
		glfwPollEvents();
	}
	defShader.Delete();
	triangleTexture.Delete();
	triangleVBO.Delete();
	triangleVAO.Delete();
	return 0;
}
