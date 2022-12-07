#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform vec4 color;

out vec4 frag_col;

void main(){	

	gl_Position = projection * view * model * vec4(aPos.x,aPos.y,aPos.z,1.0);

	frag_col = color;
	
}