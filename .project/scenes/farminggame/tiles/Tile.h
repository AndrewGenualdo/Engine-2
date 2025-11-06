//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_TILE_H
#define ENGINE_2_TILE_H

#include "../FarmingObject.h"

using namespace glm;
using namespace cobb;

class Tile : public FarmingObject {
public:
    enum TileType {
        PLANT,
        MISC
    };

    class TileData : public ObjectData {
    public:
        TileType tileType;

        explicit TileData(const TileType tileType) : ObjectData(TILE) {
            this->tileType = tileType;
        }

    };

    Tile() = default;
    Tile(ObjectType type, ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};


#endif //ENGINE_2_TILE_H