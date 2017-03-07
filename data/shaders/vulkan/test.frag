#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 1) uniform UniformBufferObject {
    vec4 color;
} ubo;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = ubo.color;
}