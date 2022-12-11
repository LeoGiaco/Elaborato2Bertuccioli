#version 330 core

in vec4 color;

uniform int selected;

out vec4 FragColor;

void main()
{
	FragColor = mix(color, vec4(1.0, 0.5, 0.0, 1.0), selected * 0.3);
}