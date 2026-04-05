#version 330 core
layout (location = 0) in vec3 ourPosition;
layout (location = 1) in vec2 texCoord;

out vec2 TextureCoordinates;

void main()
{
    gl_Position = vec4(ourPosition, 1.0);
    TextureCoordinates = texCoord;


}