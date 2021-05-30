
#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Transformations {
    mat4 translation;
    mat4 scale;
    mat4 rotation;
} trans;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = inColor;
    gl_Position = trans.translation * trans.scale * trans.rotation * vec4(inPosition, 0.0, 1.0);
}
