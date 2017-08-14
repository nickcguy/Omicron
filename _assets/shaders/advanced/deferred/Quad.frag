#version 450

out vec4 FinalColour;

uniform sampler2D AlbedoSpec;
uniform sampler2D Normal;
uniform sampler2D MetRouAo;
uniform sampler2D Position;
uniform sampler2D W_TexCoords;

in vec2 TexCoords;

uniform int outputBuffer;

void main() {

    vec2 texCoords = TexCoords;

	vec4 albSpc     = texture(AlbedoSpec, texCoords);
	vec3 albedo     = albSpc.rgb;
	float specular  = albSpc.a;
    vec3 normal     = texture(Normal, texCoords).rgb;
    vec4 mra_       = texture(MetRouAo, texCoords);
    vec2 wTexCoords = texture(W_TexCoords, texCoords).rg;

	float metallic  = mra_.r;
	float roughness = mra_.g;
	float ambient   = mra_.b;

	vec3 position = texture(Position, texCoords).rgb;

    if(outputBuffer == 1) {
        FinalColour = vec4(normal, 1.0);
    }else if(outputBuffer == 2) {
        FinalColour = vec4(specular, 0.0, 0.0, 1.0);
    }else if(outputBuffer == 3) {
        FinalColour = vec4(metallic, 0.0, 0.0, 1.0);
    }else if(outputBuffer == 4) {
        FinalColour = vec4(roughness, 0.0, 0.0, 1.0);
    }else if(outputBuffer == 5) {
        FinalColour = vec4(ambient, 0.0, 0.0, 1.0);
    }else if(outputBuffer == 6) {
        FinalColour = vec4(position, 1.0);
    }else if(outputBuffer == 7) {
        FinalColour = vec4(wTexCoords, 0.0, 1.0);
    }else{
        FinalColour = vec4(albedo, 1.0);
    }

}
