#version 330 core

uniform sampler2D tex;
uniform vec4 color;
uniform int width;
uniform int height;
uniform int textureTiles;
uniform uint tiles[512];


in vec2 TexCoords;

out vec4 FragColor;



void main()
{
    float x = TexCoords.x;
    float y = TexCoords.y;
    int tileX = int(x * width);
    int tileY = int(y * height);
    int tileIndex = tileY * width + tileX;
    int subTile = tileIndex % 4;
    uint tileData = tiles[tileIndex / 4];
    uint a = (tileData >> 24) & 0xFFu;
    uint b = (tileData >> 16) & 0xFFu;
    uint c = (tileData >> 8)  & 0xFFu;
    uint d = tileData & 0xFFu;
    uint tileDataFinal;
    if(subTile == 0) tileDataFinal = (tileData >> 24) & 0xFFu;
    else if(subTile == 1) tileDataFinal = (tileData >> 16) & 0xFFu;
    else if(subTile == 2) tileDataFinal = (tileData >> 8) & 0xFFu;
    else tileDataFinal = tileData & 0xFFu;

    float xThroughTextureTile = x * width - floor(x * width);
    float yThroughTextureTile = y * height - floor(y * height);
    float xOff = tileDataFinal * (1.0f / textureTiles);
    float finalXPos = xOff + (xThroughTextureTile / textureTiles);
    float finalYPos = yThroughTextureTile;
    FragColor = texture(tex, vec2(finalXPos, finalYPos));
    //FragColor = vec4(tiles[0] / 6.0f, tiles[1] / 6.0f, tiles[2] / 6.0f, tiles[3] / 6.0f);
}