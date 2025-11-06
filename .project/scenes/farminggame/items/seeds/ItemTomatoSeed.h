//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEMTOMATOSEED_H
#define ENGINE_2_ITEMTOMATOSEED_H

#include "ItemSeed.h"

class ItemTomatoSeed : public ItemSeed {
public:
    ItemTomatoSeed() : ItemSeed() {}
    explicit ItemTomatoSeed(ivec2 tile);
    explicit ItemTomatoSeed(vec2 pos);

    void draw(bool bind) override;
    void tick() override;
    void update(float dt) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMTOMATOSEED_H