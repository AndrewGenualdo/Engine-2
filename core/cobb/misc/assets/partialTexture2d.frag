#version 330 core

uniform sampler2D tex;
uniform vec4 color;
uniform vec4 partial; //x = first x, y = first y, z = width, w = height

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    float x = partial.x + TexCoords.x / partial.z;
    float y = partial.y + TexCoords.y / partial.w;
    if(x > 1 || y > 1 || x < 0 || y < 0) FragColor = vec4(1, 0, 0, 1);
    else FragColor = texture(tex, vec2(x, y)) * color;
}