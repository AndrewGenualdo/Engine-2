//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_TILE_H
#define ENGINE_2_TILE_H

#include "../FarmingObject.h"

using namespace glm;
using namespace cobb;

class Tile : public FarmingObject {

protected:
    bool tileExists;

public:

    class TileData : public ObjectData {
    public:

        TileData() : ObjectData() {}
        explicit TileData(const std::string &configKey, const TypeID type, const TypeID parent) : ObjectData(configKey, type, parent) {}
        ~TileData() override = default;
    };

    Tile() = default;
    explicit Tile(ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    [[nodiscard]] TypeID getType() const override;
    bool destroy() override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

    bool exists() const;

};


#endif //ENGINE_2_TILE_H