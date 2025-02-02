#version 430 core

layout(location=0) in vec2 vertex;
layout(location=1) in vec2 texcoord_in;
out vec2 texcoord;

void main() {
    gl_Position = vec4(vertex, 0.0, 1.0);
    texcoord = texcoord_in;
}