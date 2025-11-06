//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEMCARROTSEED_H
#define ENGINE_2_ITEMCARROTSEED_H
#include "ItemSeed.h"


class ItemCarrotSeed : public ItemSeed {

public:
    ItemCarrotSeed() : ItemSeed() {}
    explicit ItemCarrotSeed(ivec2 tile);
    explicit ItemCarrotSeed(vec2 pos);
    ~ItemCarrotSeed() override;

    void draw(bool bind) override;
    void tick() override;
    void update(float dt) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};
#endif //ENGINE_2_ITEMCARROTSEED_H
