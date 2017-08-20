#version 450

out vec4 depth;

uniform sampler2D shadowMap;
uniform sampler2D positionBuffer;

uniform mat4 lightSpace;

in vec2 TexCoords;

void main() {
    vec3 fragPos = texture(positionBuffer, TexCoords).rgb;

    vec4 FragPosLightSpace = lightSpace * vec4(fragPos, 1.0);

    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    depth = vec4(shadow);
}