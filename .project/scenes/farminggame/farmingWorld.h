//
// Created by cobble on 11/3/2025.
//

#ifndef ENGINE_2_WORLD_H
#define ENGINE_2_WORLD_H
#include <string>
#include <fstream>

class FarmingWorld {
public:
    constexpr static int TILES_HORIZ = 35;
    constexpr static int TILES_VERT = 17;
    unsigned int landData[TILES_HORIZ * TILES_VERT]{};
    unsigned int structureData[TILES_HORIZ * TILES_VERT]{};

    int tilesHoriz;
    int tilesVert;
    std::string savePath;

    FarmingWorld();
    FarmingWorld(int tilesHoriz, int tilesVert, const std::string &savePath);
    ~FarmingWorld();

    void load();
    void save() const;

    void cleanup();

    [[nodiscard]] unsigned int getLandData(int index) const;
    void setLandData(int index, unsigned int value);

    [[nodiscard]] unsigned int getStructureData(int index) const;
    void setStructureData(int index, unsigned int value);
};


#endif //ENGINE_2_WORLD_H