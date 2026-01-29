#ifndef SETUP
#define SETUP

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Window {
private:
	const char* m_WINDOW_NAME;
	unsigned int m_width;
	unsigned int m_height;
	GLfloat m_RED, m_GREEN, m_BLUE;
	GLFWwindow* m_window = nullptr;
public:
	Window(const char* WINDOW_NAME, unsigned int width, unsigned int height, GLfloat RED, GLfloat GREEN, GLfloat BLUE);
	void glfwSetup();
	GLFWwindow* getWindow();
	void glClearCurrentColor();
	~Window();
};

#endif
