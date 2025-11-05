//
// Created by cobble on 11/3/2025.
//

#ifndef ENGINE_2_WORLD_H
#define ENGINE_2_WORLD_H
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <bitset>
#include <vector>
#include "glm/vec2.hpp"
#include "cobb/misc/texture2d.h"
#include "cobb/misc/tiles2d.h"

using namespace cobb;
using namespace glm;

class LittleGuy;

class FarmingWorld {
public:
    constexpr static int EMPTY = 0;
    constexpr static int FARMLAND = 1;
    constexpr static int FARMLAND_TEXTURE_OFFSET = 16;
    constexpr static int MUD = 2;
    constexpr static int GRASS = 3;
    constexpr static int WATER = 4;
    constexpr static int STONE_PATH = 5;
    constexpr static int STONE_PATH_2 = 6;

    constexpr static int TILE_OFFSET_X = 100;
    constexpr static int TILE_OFFSET_Y = 100;
    constexpr static int TILE_WIDTH = 75;
    constexpr static int TILE_HEIGHT = 75;

    constexpr static int TEXTURE_TILE_COUNT = 16;

    constexpr static int TILES_HORIZ = 25;
    constexpr static int TILES_VERT = 12;

    unsigned int landData[TILES_HORIZ * TILES_VERT];
    unsigned int plantData[TILES_HORIZ * TILES_VERT];

    static Tiles2d landTilemap;
    static Tiles2d plantTilemap;

    struct Tile {
        int landType = 0;
        int plantType = 0;

        Tile() = default;
        Tile(int landType, int structureType) {
            this->landType = landType;
            this->plantType = structureType;
        }
    };

    Tile tiles[TILES_HORIZ * TILES_VERT]{};

    std::string savePath;

    std::vector<LittleGuy> guys;

    FarmingWorld();
    explicit FarmingWorld(const std::string &savePath);
    ~FarmingWorld();

    void load();
    void save() const;
    void cleanup();

    void update(float dt);
    void draw() const;

    void updateTileTypes();
    static bool isFarmland(int texIndex);
    [[nodiscard]] bool isFarmland(int x, int y) const;

    [[nodiscard]] Tile getTile(int x, int y) const;
    [[nodiscard]] static vec2 getTilePos(int x, int y);
    [[nodiscard]] static ivec2 getTileFromPos(vec2 pos);

    [[nodiscard]] unsigned int getLandData(int index) const;
    void setLandData(int index, unsigned int value);

    [[nodiscard]] unsigned int getStructureData(int index) const;
    void setStructureData(int index, unsigned int value);
};


#endif //ENGINE_2_WORLD_H