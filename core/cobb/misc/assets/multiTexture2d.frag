#version 330 core

uniform sampler2D tex;
uniform vec4 color;
uniform int frames;
uniform int frame;

in vec2 TexCoords;

out vec4 FragColor;



void main()
{
    FragColor = texture(tex, vec2((TexCoords.x + float(frame)) / float(frames), TexCoords.y)) * color;
}