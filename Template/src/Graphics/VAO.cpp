#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset); // specify the layout of the vertex data
	// 0 is the index of the vertex attribute (location = 0 in the VAO)
	// 3 is the number of components per vertex attribute (x, y, opacity)
	// GL_FLOAT is the data type of each component
	// GL_FALSE keep the original array values 255 -> 255 GL_TRUE would map it to 1.0
	// 3 * sizeof(float) is the stride (the byte offset between consecutive vertex attributes) aka distance between each vertex attribute
	// (void*)0 -> (nullptr) -> offset of the first component of the first vertex attribute in the buffer aka where the data begins in the buffer
	glEnableVertexAttribArray(layout); // enable the VBO at index 0 in VAO
	VBO.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);

}