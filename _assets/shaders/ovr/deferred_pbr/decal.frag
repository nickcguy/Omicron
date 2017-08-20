#version 450

uniform sampler2D AlbedoMap;
uniform sampler2D AmbientMap;
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;
uniform sampler2D RefractiveMap;
uniform sampler2D DepthBuffer;

in vec4 PositionOS;
in vec4 PositionWS;
in vec4 PositionVS;
in vec4 PositionCS;

in mat4 InverseView;
in mat4 InverseModel;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 metRouAo;
layout(location = 3) out vec4 position;
layout(location = 4) out vec4 distortion;

uniform float UVScale = 1.0;
uniform float farClip = 1024.0;

void main() {

    if(0.5 > abs(PositionOS.xyz))
        discard;

    vec2 texCoords = PositionOS.xz * 0.5 + 0.5;


    albedo = texture(AlbedoMap, texCoords);
    position = vec4(PositionWS.xyz, 1.0);
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
