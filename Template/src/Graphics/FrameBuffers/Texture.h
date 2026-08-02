#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <iostream>
#include <string>

#include<glad/glad.h>

#include"../Shaders/ShaderManager.h"

class Texture
{
public:
	GLuint ID;
	std::string type;
	std::string path;
	GLuint unit;

	Texture(const std::string& image, const std::string& texType, GLuint slot);

	void texUnit(const std::string& shaderName, const std::string& uniform);
	void texUnit(Shader& shader, const std::string&, GLuint unit); // Old
	void Bind();
	void Unbind();
	void Delete();
};
#endif