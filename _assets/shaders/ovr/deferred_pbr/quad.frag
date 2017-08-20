#version 450

out vec4 FinalColour;

uniform sampler2D AlbedoSpec;
uniform sampler2D Normal;
uniform sampler2D MetRouAo;
uniform sampler2D Position;
uniform sampler2D Distortion;
uniform sampler2D Skybox;
uniform sampler2D ShadowMap;

in vec2 TexCoords;

uniform int outputBuffer;
uniform vec3 viewPos;

struct Light {
    bool Active;

    int Type;

    vec3 Position;
    vec3 Direction;
    vec3 Colour;

    float Linear;
    float Quadratic;
    float Constant;

    float Cutoff;
    float OuterCutoff;

    float Intensity;
};

const int DIRECTIONAL = 0;
const int POINT = 1;
const int SPOT = 2;

const float PI = 3.14159265359;
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

vec3 CalculateLight(vec3 radiance, vec3 norm, vec3 H, vec3 viewDir, vec3 L, vec3 Diffuse, float metallic, float roughness, vec3 F0) {
    float NDF = DistributionGGX(norm, H, roughness);
    float G = GeometrySmith(norm, viewDir, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4 * max(dot(norm, viewDir), 0.0) * max(dot(norm, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(norm, L), 0.0);
    return (kD * Diffuse / PI + specular) * radiance * NdotL;
}

vec3 CalculateDirectionalLight(Light light, vec3 norm, vec3 viewDir, vec3 Diffuse, float metallic, float roughness, vec3 F0);
vec3 CalculatePointLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float metallic, float roughness, vec3 F0);
vec3 CalculateSpotLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float metallic, float roughness, vec3 F0);

void main() {

//    vec4 distortion = texture(Distortion, TexCoords);
    vec2 texCoords = TexCoords;

//    vec2 texCoords = vec2(TexCoords.x + (distortion.r * distortion.b), TexCoords.y + (distortion.g * distortion.b));

	vec4 albSpc     = texture(AlbedoSpec, texCoords);
	vec3 Diffuse     = albSpc.rgb;
	float Specular  = albSpc.a;
    vec3 normal     = texture(Normal, texCoords).rgb;

    vec4 mra_ = texture(MetRouAo, texCoords);

	float metallic  = mra_.r;
	float roughness = mra_.g;
	float Ambient   = mra_.b;

//    float metallic  = 0.4;
//    float roughness = 0.7;
//    float Ambient   = 0.1;

	vec3 position = texture(Position, texCoords).rgb;

	vec3 N = normalize(normal);
	vec3 V = normalize(viewPos - position);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, Diffuse, metallic);

	vec3 Lo = vec3(0.0);
	for(int i = 0; i < NR_LIGHTS; i++) {
	    Light light = lights[i];
	    if(!light.Active) continue;

	    if(light.Type == DIRECTIONAL) {
	        Lo += CalculateDirectionalLight(light, N, V, Diffuse, metallic, roughness, F0);
	    }else if(light.Type == POINT) {
	        Lo += CalculatePointLight(light, N, position, V, Diffuse, metallic, roughness, F0);
	    }else if(light.Type == SPOT) {
	        Lo += CalculateSpotLight(light, N, position, V, Diffuse, metallic, roughness, F0);
	    }
	}

	vec3 ambient = vec3(0.03) * Diffuse;
	vec3 colour = ambient + Lo;

	float shadow = texture(ShadowMap, TexCoords).r;

//	colour *= texture(ShadowMap, TexCoords).r;

//	const float gamma = 2.2;
//    colour = colour / (colour + vec3(1.0));
//    colour = pow(colour, vec3(1.0 / gamma));

    vec4 skybox = texture(Skybox, TexCoords);
    FinalColour = skybox;
    FinalColour.a = 1.0;

    if(outputBuffer == 2) {
        FinalColour.rgb += Diffuse;
    }else if(outputBuffer == 3) {
        FinalColour.rgb += Lo;
    }else if(outputBuffer == 4) {
        FinalColour.rgb += vec3(metallic);
    }else if(outputBuffer == 5) {
        FinalColour.rgb += vec3(roughness);
    }else if(outputBuffer == 6) {
        FinalColour.rgb += vec3(Ambient);
    }else if(outputBuffer == 7) {
        FinalColour.rgb += position;
    }else if(outputBuffer == 8) {
        FinalColour.rgb += N;
    }else if(outputBuffer == 9) {
        FinalColour.rgb += skybox.rgb;
    }else if(outputBuffer == 1) {
        FinalColour.rgb += vec3(shadow);
    }else{
	    FinalColour.rgb += colour;
	}

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

vec3 CalculateDirectionalLight(Light light, vec3 norm, vec3 viewDir, vec3 Diffuse, float metallic, float roughness, vec3 F0) {
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 radiance = (light.Colour * light.Intensity) * diff;

    vec3 H = normalize(viewDir + lightDir);

    return CalculateLight(radiance, norm, H, viewDir, lightDir, Diffuse, metallic, roughness, F0);
}

vec3 CalculatePointLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float metallic, float roughness, vec3 F0) {
    vec3 L = normalize(light.Position - fragPos);
	vec3 H = normalize(viewDir + L);
	float distance = length(light.Position - fragPos);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = (light.Colour * light.Intensity) * attenuation;

	return CalculateLight(radiance, norm, H, viewDir, L, Diffuse, metallic, roughness, F0);
}

vec3 CalculateSpotLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 Diffuse, float metallic, float roughness, vec3 F0) {
    vec3 L = normalize(light.Position - fragPos);
    vec3 H = normalize(viewDir + L);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    float theta = dot(L, normalize(-light.Direction));
    float epsilon = light.Cutoff - light.OuterCutoff;
    float intensity = 1.0 - clamp((theta - light.OuterCutoff) / epsilon, 0.0, 1.0);

    vec3 radiance = (light.Colour * light.Intensity) * attenuation * intensity;

    return CalculateLight(radiance, norm, H, viewDir, L, Diffuse, metallic, roughness, F0);
}