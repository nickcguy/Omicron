#version 450

layout(location = 0) out vec4 albedoSpec;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 metRouAo; // packed metallic/roughness/ambient
layout(location = 3) out vec4 position;
layout(location = 4) out vec4 wTexCoords;

uniform sampler2D AlbedoMap;
uniform sampler2D AmbientMap;
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;

uniform vec3 Position;
uniform vec2 TexCoords;
uniform vec3 Normal;
uniform vec3 WorldNormal;
uniform vec4 Colour;

void main() {

    vec2 texCoords = TexCoords;

    albedoSpec = texture(AlbedoMap, texCoords);
//    albedoSpec.rgb *= Colour.rgb;
    albedoSpec.a = 1.0;

    normal.rgb = texture(NormalMap, texCoords).rgb * normalize(Normal);
    normal.a = 1.0;

    metRouAo.r = texture(MetallicMap, texCoords).r;
    metRouAo.g = texture(RoughnessMap, texCoords).r;
    metRouAo.b = texture(AmbientMap, texCoords).r;
    metRouAo.a = 1.0;

    position = vec4(Position, 1.0);

    wTexCoords = vec4(TexCoords, 0.0, 1.0);

}
