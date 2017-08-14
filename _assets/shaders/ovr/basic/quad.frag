#version 450

in vec2 TexCoords;

uniform sampler2D image;

out vec4 FinalColour;

void main() {
	vec4 imgTex = texture(image, TexCoords);
//	FinalColour = imgTex;
	FinalColour = vec4(TexCoords, 0.0, 1.0);
}
