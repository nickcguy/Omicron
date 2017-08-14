#version 450

uniform sampler2D AlbedoMap;
uniform sampler2D AmbientMap;
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;

in vec4 Colour;
in vec3 Normal;
in vec3 WorldNormal;
in vec2 TexCoords;
in vec3 Position;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 position;
layout(location = 2) out vec4 normal;
layout(location = 3) out vec4 packed;


uniform int outputBuffer = 0;

void main() {

    albedo = texture(AlbedoMap, TexCoords);
    position = vec4(Position, 1.0);
    normal.rgb = texture(NormalMap, TexCoords).rgb * normalize(Normal);
    normal.a = 1.0;

    packed.r = texture(MetallicMap, TexCoords).r;
    packed.g = texture(RoughnessMap, TexCoords).r;
    packed.b = texture(AmbientMap, TexCoords).r;

//    if(buf == 0) {
//      	FinalColour = texture(AlbedoMap, TexCoords);
//    }else if(buf == 1) {
//        FinalColour = texture(AmbientMap, TexCoords);
//    }else if(buf == 2) {
//        FinalColour = texture(MetallicMap, TexCoords);
//    }else if(buf == 3) {
//        FinalColour = texture(NormalMap, TexCoords);
//    }else if(buf == 4) {
//        FinalColour = texture(RoughnessMap, TexCoords);
//    }else{
//        FinalColour = Colour;
//    }

}
