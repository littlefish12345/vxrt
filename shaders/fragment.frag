#version 430 core

#define GAMMA 2.2f

in vec2 texcoord;
out vec4 FragColor;
uniform sampler2D output_image;

void main() {
    FragColor = vec4(pow(texture(output_image, texcoord).xyz, vec3(1/GAMMA, 1/GAMMA, 1/GAMMA)), 1.0f);
}