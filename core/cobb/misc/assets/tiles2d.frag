#version 330 core

uniform sampler2D tex;
uniform vec4 color;
uniform int width;
uniform int height;
const int textureTiles = 64;
uniform int tiles[textureTiles];


in vec2 TexCoords;

out vec4 FragColor;



void main()
{
    /*int tileIndex = int(TexCoords.x * maxTiles);
    int tileX = tileIndex % width;
    int tileY = tileIndex / width;
    vec4 color = texture(tex, vec2((TexCoords.x + float(frame)) / float(frames), TexCoords.y)) * color;
    FragColor = texture(tex, vec2((TexCoords.x + float(frame)) / float(frames), TexCoords.y)) * color;*/
    float x = TexCoords.x;
    float y = TexCoords.y;
    int tileX = int(x * width);
    int tileY = int(y * height);
    int tileIndex = tileX * width + tileY;
    int tileData = tiles[tileIndex];
    float xThroughTextureTile = x * width - floor(x * width);
    float yThroughTextureTile = y * height - floor(y * height);
    float xOff = tileData * (1 / textureTiles);
    float finalXPos = xOff + (xThroughTextureTile / textureTiles);
    float finalYPos = yThroughTextureTile;
    FragColor = texture(tex, vec2(finalXPos, finalYPos));
}