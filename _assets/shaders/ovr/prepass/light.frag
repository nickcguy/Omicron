#version 450

out vec4 FinalColour;

uniform sampler2D AlbedoSpec;
uniform sampler2D Normal;
uniform sampler2D MetRouAo;
uniform sampler2D Position;

in vec2 TexCoords;

uniform int outputBuffer;
uniform vec3 viewPos;

struct Light {
    vec3 Position;
    vec3 Colour;

    float Constant;
    float Linear;
    float Quadratic;

    float Intensity;
};

const float PI = 3.14159265359;
const int NR_LIGHTS = 16;
uniform Light lights[NR_LIGHTS];

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

void main() {

    vec2 texCoords = TexCoords;

	vec4 albSpc     = texture(AlbedoSpec, texCoords);
	vec3 Diffuse     = albSpc.rgb;
	float Specular  = albSpc.a;
    vec3 normal     = texture(Normal, texCoords).rgb;

	float metallic  = texture(MetRouAo, texCoords).r;
	float roughness = texture(MetRouAo, texCoords).g;
	float Ambient   = texture(MetRouAo, texCoords).b;

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
	    vec3 L = normalize(light.Position - position);
	    vec3 H = normalize(V + L);
	    float distance = length(light.Position - position);
	    float attenuation = 1.0 / (distance * distance);
	    vec3 radiance = (light.Colour * light.Intensity) * attenuation;

	    float NDF = DistributionGGX(N, H, roughness);
	    float G = GeometrySmith(N, V, L, roughness);
	    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

	    vec3 kS = F;
	    vec3 kD = vec3(1.0) - kS;
	    kD *= 1.0 - metallic;

	    vec3 numerator = NDF * G * F;
	    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
	    vec3 specular = numerator / denominator;

	    float NdotL = max(dot(N, L), 0.0);
	    Lo += (kD * Diffuse / PI + specular) * radiance * NdotL;
	}

    FinalColour = vec4(Lo, 1.0);
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