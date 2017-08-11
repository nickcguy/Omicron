#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

uniform mat4 projection;
uniform mat4 view;

out vec3 Colour;


void main() {
	Colour = colour;
    gl_Position = projection * view * vec4(position, 1.0);
}