#version 330 core
uniform sampler2D u_Atlas;
in vec2 TextureCoordinates;
out vec4 fragColor;

void main() {
    fragColor = texture(u_Atlas, TextureCoordinates);
}