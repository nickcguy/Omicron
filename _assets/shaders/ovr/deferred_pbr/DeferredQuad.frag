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

    float Linear;
    float Quadratic;
    float Radius;
};

const int NR_LIGHTS = 1;
Light lights[NR_LIGHTS];

void main() {

    lights[0].Position = vec3(0.0, 2.4, 10.0);
    lights[0].Colour = vec3(150.0, 150.0, 150.0);

    float maxBrightness = max(lights[0].Colour.r, max(lights[0].Colour.g, lights[0].Colour.b));

    lights[0].Linear = 0.7;
    lights[0].Quadratic = 1.8;
    lights[0].Radius = (-lights[0].Linear + sqrt(lights[0].Linear * lights[0].Linear - 4 * lights[0].Quadratic * (1.0 - (256.0f / 5.0f) * maxBrightness))) / (2.0f * lights[0].Quadratic);

    vec2 texCoords = TexCoords;

	vec4 albSpc     = texture(AlbedoSpec, texCoords);
	vec3 Diffuse     = albSpc.rgb;
	float Specular  = albSpc.a;
    vec3 normal     = texture(Normal, texCoords).rgb;
    vec4 mra_       = texture(MetRouAo, texCoords);

	float metallic  = mra_.r;
	float roughness = mra_.g;
	float ambient   = mra_.b;

	vec3 position = texture(Position, texCoords).rgb;

	vec3 lighting = Diffuse * 0.1;
	vec3 viewDir = normalize(viewPos - position);
	for(int i = 0; i < NR_LIGHTS; i++) {
	    Light light = lights[i];
	    float distance = length(light.Position - position);
	    if(distance < light.Radius) {
	        vec3 lightDir = normalize(light.Position - position);
	        vec3 diffuse = max(dot(normal, lightDir), 0.0) * Diffuse * light.Colour;

	        vec3 halfwayDir = normalize(lightDir + viewDir);
	        float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	        vec3 specular = light.Colour * spec * Specular;

	        float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
	        diffuse *= attenuation;
	        specular *= attenuation;
	        lighting += diffuse + specular;
	    }
	}

	FinalColour = vec4(lighting, 1.0);

}
