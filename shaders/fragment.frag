#version 430 core

in vec2 texcoord;
out vec4 FragColor;
uniform sampler2D output_image;

void main() {
    FragColor = texture(output_image, texcoord);
}