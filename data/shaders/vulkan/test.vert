#version 450
#extension GL_ARB_separate_shader_objects : enable

//layout(binding = 0) uniform UniformBufferObject {
//    mat4 MVP;
//} ubo;


layout (std140, push_constant) uniform PushConsts 
{
	mat4 MVP;
} pushConsts;

layout(location = 0) in vec3 inPosition;


void main() {
    gl_Position = pushConsts.MVP * vec4(inPosition, 1.0);
}