#version 450

// Vertex attributes
// These properties are specified per-vertex in the vertex buffer
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

// Some types, like dvec3 64 bit vectors, use multiple slots. The next index must be 2 higher
// layout(location = 0) in dvec3 inPosition;
// layout(location = 2) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}