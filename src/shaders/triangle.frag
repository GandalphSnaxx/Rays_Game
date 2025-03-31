#version 460 core

layout(location = 0) in vec3 a_FragColor;

layout(location = 0) out vec4 out_Color;

void main() {
	out_Color = vec4(a_FragColor, 1.0);
}