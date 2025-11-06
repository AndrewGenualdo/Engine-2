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

#include "FarmingObject.h"
#include "glm/vec2.hpp"
#include "cobb/misc/texture2d.h"
#include "cobb/misc/tiles2d.h"
#include "tiles/Tile.h"

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

    constexpr static int TILE_OFFSET_X = 55;
    constexpr static int TILE_OFFSET_Y = 65;
    constexpr static int TILE_WIDTH = 75;
    constexpr static int TILE_HEIGHT = 75;

    constexpr static int TEXTURE_TILE_COUNT = 16;

    constexpr static int TILES_HORIZ = 24;
    constexpr static int TILES_VERT = 13;

    constexpr static int TICKS_PER_SECOND = 10;

    unsigned int landData[TILES_HORIZ * TILES_VERT];
    unsigned int plantData[TILES_HORIZ * TILES_VERT];

    static Tiles2d landTilemap;
    static Tiles2d plantTilemap;

    std::string savePath;

    std::vector<LittleGuy*> guys;
    std::vector<FarmingObject*> objects;

    FarmingWorld();
    explicit FarmingWorld(const std::string &savePath);
    ~FarmingWorld();

    void load();
    void save() const;
    void cleanup();

    void update(float dt);
    void tick();
    void draw();

    void updateTileTypes();
    void clearObjects();

    static bool isFarmland(int texIndex);
    [[nodiscard]] bool isFarmland(int x, int y) const;

    [[nodiscard]] static vec2 getTilePos(int x, int y);
    [[nodiscard]] static ivec2 getTileFromPos(vec2 pos);
};


#endif //ENGINE_2_WORLD_H