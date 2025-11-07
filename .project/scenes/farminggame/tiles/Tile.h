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

    class TileData : public ObjectData {
    public:

        TileData() : ObjectData() {}
        explicit TileData(const TypeID type, const TypeID parent) : ObjectData(type, parent) {}
        ~TileData() override = default;

    };

    Tile() = default;
    explicit Tile(ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};


#endif //ENGINE_2_TILE_H