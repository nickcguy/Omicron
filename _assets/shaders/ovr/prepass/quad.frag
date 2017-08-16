#version 450

out vec4 FinalColour;

uniform sampler2D AlbedoSpec;
uniform sampler2D Lighting;

in vec2 TexCoords;

uniform int outputBuffer;

void main() {

    vec2 texCoords = TexCoords;

	vec3 Diffuse = texture(AlbedoSpec, texCoords).rgb;
	vec3 Lo = texture(Lighting, texCoords).rgb;

	vec3 ambient = vec3(0.03) * Diffuse;
	vec3 colour = ambient + Lo;

    if(outputBuffer == 2) {
        FinalColour = vec4(Lo, 1.0);
    }else if(outputBuffer == 3) {
        FinalColour = vec4(Diffuse, 1.0);
    }else if(outputBuffer == 4) {
        FinalColour = vec4(ambient, 1.0);
    }else{
        FinalColour = vec4(colour, 1.0);
    }

}