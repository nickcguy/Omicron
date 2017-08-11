#version 450

in vec3 Colour;

out vec4 FinalColour;

void main() {
    FinalColour = vec4(Colour, 1.0);
}
