#include "main.h"
constexpr unsigned int width = 900;
constexpr unsigned int height = 900;

int main()
{
	std::cout << "Hello CMake." << std::endl;

	// Name of the window, width & height of the window, background color RGB
	Window VIEWPORT("Template", width, height, 0.7f, 0.7f, 0.7f);
	VIEWPORT.glfwSetup();

	Shader defShader("shaders/default.vert", "shaders/default.frag");

	while (!glfwWindowShouldClose(VIEWPORT.getWindow())) {
		VIEWPORT.glClearCurrentColor();
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(VIEWPORT.getWindow());
		glfwPollEvents();
	}
	return 0;
}
