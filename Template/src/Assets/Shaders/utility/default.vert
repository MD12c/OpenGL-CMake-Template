#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 scale;
uniform vec3 camPos;

void main(){
	gl_Position = proj * view * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
}