//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEMTOMATOSEED_H
#define ENGINE_2_ITEMTOMATOSEED_H

#include "ItemSeed.h"

class ItemSeedTomato : public ItemSeed {
public:
    ItemSeedTomato() : ItemSeed() {}
    explicit ItemSeedTomato(ivec2 tile);
    explicit ItemSeedTomato(vec2 pos);

    void draw(bool bind) override;
    void tick() override;
    void update(float dt) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMTOMATOSEED_H