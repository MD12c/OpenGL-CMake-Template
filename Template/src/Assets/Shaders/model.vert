#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

out DATA
{
    vec3 Position;
    vec3 Normal;
    vec2 texCoord;
    mat4 camMatrix;
}
data_out;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal;

void main()
{
    data_out.Position  = vec3(model * vec4(aPos, 1.0f));
    data_out.Normal    = normal * aNormal;
    data_out.texCoord  = aTex;
    data_out.camMatrix = proj * view;
}