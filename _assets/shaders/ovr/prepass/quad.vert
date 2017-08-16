#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;
out float vertId;

void main() {
    TexCoords = texCoords;
    vertId = float(gl_VertexID);
	gl_Position = vec4(position, 1.0);
}
