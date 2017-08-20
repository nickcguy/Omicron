#version 450

uniform sampler2D AlbedoMap;
uniform sampler2D AmbientMap;
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;
uniform sampler2D RefractiveMap;

in vec4 Colour;
in vec3 Normal;
in vec3 WorldNormal;
in vec2 TexCoords;
in vec3 Position;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 metRouAo;
layout(location = 3) out vec4 position;
layout(location = 4) out vec4 distortion;

uniform float UVScale = 1.0;

void main() {

    vec2 texCoords = TexCoords;

    texCoords.x *= UVScale;
    texCoords.y *= UVScale;

    texCoords.x = mod(texCoords.x, 1.0);
    texCoords.y = mod(texCoords.y, 1.0);

    albedo = texture(AlbedoMap, texCoords);
    position = vec4(Position, 1.0);
    normal.rgb = texture(NormalMap, texCoords).rgb * WorldNormal;
    normal.a = 1.0;

    metRouAo.r = texture(MetallicMap, texCoords).r;
    metRouAo.g = texture(RoughnessMap, texCoords).g;
    metRouAo.b = texture(AmbientMap, texCoords).b;
    metRouAo.a = 1.0;

    distortion.rgb = vec3(0.0);
    distortion.a = texture(RefractiveMap, texCoords).r;

//    metallic = vec4(0.4, 0.4, 0.4, 1.0);
//    roughness = vec4(vec3(0.7), 1.0);
//    ao = vec4(vec3(0.1), 1.0);

}
