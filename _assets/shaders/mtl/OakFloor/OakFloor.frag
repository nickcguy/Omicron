#version 450

#define USE_SSBO true

in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldNormal;
in vec4 Colour;

out vec4 FinalColour;

uniform sampler2D AlbedoMap;
uniform sampler2D AmbientMap;
uniform sampler2D HeightMap;
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;

uniform vec3 camPos;
uniform vec3 camDir;

uniform float AlbedoScalar;
uniform float Gamma;
uniform float HeightScale;
uniform float Overlap;
uniform float UVScale;

const float PI = 3.14159265359;

uniform int outputBuffer;

layout(std430, binding = 0) buffer LightBuffer {
    int lightCount;
    int lightStride;
    int lightPositionOffset;
    int lightColourOffset;
    float lightData[];
};

#if !USE_SSBO
uniform vec3 lightPositions[16];
uniform vec3 lightColours[16];
#endif

vec3 GetNormalFromMap(vec2 texCoords) {
    vec3 tangentNormal = texture(NormalMap, texCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(Position);
    vec3 Q2 = dFdy(Position);
    vec2 st1 = dFdx(texCoords);
    vec2 st2 = dFdy(texCoords);

    vec3 N =  normalize(Normal);
    vec3 T =  normalize(Q1*st2.t - Q2*st1.t);
    vec3 B = -normalize(cross(N, T));

    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    float height = texture(HeightMap, texCoords).r;
    vec2 p = viewDir.xy / viewDir.z * (height * HeightScale);
    return texCoords - p;
}

void main() {

    vec2 texCoords = TexCoords * UVScale;

    vec3 albedo = pow(texture(AlbedoMap, texCoords).rgb, vec3(AlbedoScalar));
    float metallic = 0.0;
    float roughness = texture(RoughnessMap, texCoords).r;
    float ao = texture(AmbientMap, texCoords).r;

    vec3 N = GetNormalFromMap(texCoords);
//    vec3 N = normalize(texture(NormalMap, TexCoords).xyz * 2.0 - 1.0);
    vec3 V = normalize(camPos - Position);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    #if !USE_SSBO
    int lightCount = 16;
    #endif

    for(int i = 0; i < lightCount; i++) {

        // Extract data from SSBO
        int startIndex = i * lightStride;

        #if USE_SSBO
        vec3 lightPosition = vec3(0.0);
        lightPosition.x = lightData[startIndex + lightPositionOffset + 0];
        lightPosition.y = lightData[startIndex + lightPositionOffset + 1];
        lightPosition.z = lightData[startIndex + lightPositionOffset + 2];

        vec3 lightColour = vec3(0.0);
        lightColour.r = lightData[startIndex + lightColourOffset + 0];
        lightColour.g = lightData[startIndex + lightColourOffset + 1];
        lightColour.b = lightData[startIndex + lightColourOffset + 2];
        #else
        vec3 lightPosition = lightPositions[i];
        vec3 lightColour = lightColours[i];
        #endif

        // Calculate per light radiance
        vec3 L = normalize(lightPosition - Position);
        vec3 H = normalize(V + L);
        float distance = length(lightPosition - Position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColour * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
        vec3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = nominator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 colour = ambient + Lo;
    colour = colour / (colour + vec3(1.0));
    colour = pow(colour, vec3(1.0 / Gamma));

    colour.rg += mod(texCoords, vec2(1.0)) * Overlap;

    if(outputBuffer == 1)
        FinalColour = vec4(albedo, 1.0);
    else if(outputBuffer == 2)
        FinalColour = vec4(metallic, roughness, ao, 1.0);
    else if(outputBuffer == 3)
        FinalColour = vec4(N, 1.0);
    else if(outputBuffer == 4)
        FinalColour = vec4(Lo, 1.0);
    else
        FinalColour = vec4(colour, 1.0);


}