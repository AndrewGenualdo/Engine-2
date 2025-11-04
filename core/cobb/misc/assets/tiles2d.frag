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
    float texTiles = float(textureTiles);
    int tileX = int(x * width);
    int tileY = int(y * height);
    int tileIndex = tileY * width + tileX;
    int subTile = tileIndex % 4;
    uint tileData = tiles[tileIndex / 4];
    uint tileDataFinal;
    if(subTile == 0) tileDataFinal = (tileData >> 24) & 0xFFu;
    else if(subTile == 1) tileDataFinal = (tileData >> 16) & 0xFFu;
    else if(subTile == 2) tileDataFinal = (tileData >> 8) & 0xFFu;
    else tileDataFinal = tileData & 0xFFu;

    int tileDataX = int(mod(tileDataFinal, texTiles));
    int tileDataY = (textureTiles - 1) - int(float(tileDataFinal) / float(textureTiles));

    float xThroughTextureTile = x * width - floor(x * width);
    float yThroughTextureTile = y * height - floor(y * height);
    float xOff = tileDataX * (1.0f / texTiles);
    float yOff = tileDataY * (1.0f / texTiles);
    float finalXPos = xOff + (xThroughTextureTile / texTiles);
    float finalYPos = yOff + (yThroughTextureTile / texTiles);
    FragColor = texture(tex, vec2(finalXPos, finalYPos));
    //FragColor = vec4(finalXPos, finalYPos, 0, 1);
    //FragColor = vec4(x, y, 0, 1);
}