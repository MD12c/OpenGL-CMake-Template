#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <iostream>
#include <string>

#include<glad/glad.h>

#include"../Shader.h"

class Texture
{
public:
	GLuint ID;
	std::string type;
	std::string path;
	GLuint unit;

	Texture(std::string image, std::string texType, GLuint slot);

	void texUnit(Shader& shader, std::string uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};
#endif