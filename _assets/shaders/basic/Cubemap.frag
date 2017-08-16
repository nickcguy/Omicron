#version 450

out vec4 FinalColour;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform sampler2D gradient;
uniform float dayNight;

void main() {


    float dnVal = (dayNight * 2.0) - 1.0;
    // TODO implement proper cubemap day/night support
    vec4 grad = texture(gradient, vec2(dnVal, 0.5));

    FinalColour = texture(skybox, TexCoords);
    FinalColour.rgb *= (0.8 + (1.0 - 0.8) * dnVal);
}