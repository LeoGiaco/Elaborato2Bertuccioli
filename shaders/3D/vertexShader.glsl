#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat3 normModel;

out vec3 normalV;
out vec4 pos;

void main()
{    
    normalV = normModel * vertexNormal; // Not normalized because interpolation ruins it.

    pos = model * vec4(aPos, 1.0);
	gl_Position = projection * view * pos;
}
