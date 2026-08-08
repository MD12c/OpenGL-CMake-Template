#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 Normal;
out vec3 color;
out vec2 texCoord;
out vec3 crntPos;

in DATA
{
    vec3 Position;
    vec3 Normal;
    vec3 color;
    vec2 texCoord;
    mat4 camMatrix;
}
data_in[];

void main()
{
    gl_Position  = data_in[0].camMatrix * vec4(data_in[0].Position, 1.0f);
    Normal       = data_in[0].Normal;
    texCoord     = data_in[0].texCoord;
    crntPos      = data_in[0].Position;
    color        = data_in[0].color;
    EmitVertex();

    gl_Position  = data_in[1].camMatrix * vec4(data_in[1].Position, 1.0f);
    Normal       = data_in[1].Normal;
    texCoord     = data_in[1].texCoord;
    crntPos      = data_in[1].Position;
    color        = data_in[1].color;
    EmitVertex();

    gl_Position  = data_in[2].camMatrix * vec4(data_in[2].Position, 1.0f);
    Normal       = data_in[2].Normal;
    texCoord     = data_in[2].texCoord;
    crntPos      = data_in[2].Position;
    color        = data_in[2].color;
    EmitVertex();

    EndPrimitive();
}