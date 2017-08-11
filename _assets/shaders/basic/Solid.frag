#version 450

in vec4 Colour;
in vec3 Normal;
in vec3 WorldNormal;
in vec2 TexCoords;
in vec3 Position;

out vec4 FinalColour;

uniform float R;
uniform float G;
uniform float B;
uniform float alpha;

uniform int outputBuffer = 0;

void main() {

    int buf = clamp(outputBuffer, 0, 4);

    if(buf == 0) {
      	FinalColour = vec4(R, G, B, alpha);
    }else if(buf == 1) {
        FinalColour = vec4(Normal, 1.0);
    }else if(buf == 2) {
        FinalColour = vec4(WorldNormal, 1.0);
    }else if(buf == 3) {
        FinalColour = vec4(TexCoords, 0.0, 1.0);
    }else if(buf == 4) {
        FinalColour = vec4(Position, 1.0);
    }
}
