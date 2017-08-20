#version 450

layout(location = 0) in vec3 position;

//layout (std140) uniform Matrices {

//};


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out mat4 InverseView;
out mat4 InverseModel;

out vec4 PositionOS;
out vec4 PositionWS;
out vec4 PositionVS;
out vec4 PositionCS;

void main() {
    InverseView = inverse(view);
    InverseModel = inverse(model);

    PositionOS = vec4(position, 1.0);
    PositionWS = model * PositionOS;
	PositionVS = view * Position;
    PositionCS = projection * PositionVS;

    gl_Position = PositionCS;
}