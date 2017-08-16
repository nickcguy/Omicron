#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 colour;

//layout (std140) uniform Matrices {

//};


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 Position;
out vec2 TexCoords;
out vec3 Normal;
out vec3 WorldNormal;
out vec4 Colour;

void main() {
    vec4 worldPos = model * vec4(position, 1.0);

    gl_Position = projection * view * worldPos;

	Position = worldPos.xyz;
	TexCoords = texCoord;
	Normal = normal;
	WorldNormal = transpose(inverse(mat3(model))) * normal;
	Colour = colour;

}