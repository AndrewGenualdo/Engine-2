//
// Created by cobble on 11/3/2025.
//

#ifndef ENGINE_2_WORLD_H
#define ENGINE_2_WORLD_H
#include <string>
#include <fstream>
#include <cstdint> // for uint8_t
#include <iostream>
#include <bitset>   // <-- Needed for std::bitset

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



    constexpr static int TILES_HORIZ = 25;
    constexpr static int TILES_VERT = 12;
    unsigned int landData[TILES_HORIZ * TILES_VERT]{};
    unsigned int structureData[TILES_HORIZ * TILES_VERT]{};

    struct Tile {
        int landType = 0;
        int structureType = 0;

        Tile() = default;
        Tile(int landType, int structureType) {
            this->landType = landType;
            this->structureType = structureType;
        }
    };

    Tile tiles[TILES_HORIZ * TILES_VERT]{};

    int tilesHoriz;
    int tilesVert;
    std::string savePath;

    FarmingWorld();
    FarmingWorld(int tilesHoriz, int tilesVert, const std::string &savePath);
    ~FarmingWorld();

    void load();
    void save() const;

    void cleanup();
    void updateTileTypes();
    static bool isFarmland(int texIndex);
    bool isFarmland(int x, int y) const;


    [[nodiscard]] Tile getTile(int x, int y) const;
    [[nodiscard]] unsigned int getLandData(int index) const;
    void setLandData(int index, unsigned int value);

    [[nodiscard]] unsigned int getStructureData(int index) const;
    void setStructureData(int index, unsigned int value);
};


#endif //ENGINE_2_WORLD_H