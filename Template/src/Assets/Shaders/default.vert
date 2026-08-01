#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 color;
out vec2 TexCoord;

uniform mat4 cameraMatrix;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 model;
uniform mat4 scale;

uniform vec3 Color;

void main(){
	gl_Position = cameraMatrix * model * translation * rotation * scale * vec4(aPos, 1.0f);
	color = Color;
	TexCoord = aTexCoord;
}