//
// Created by cobble on 11/19/2025.
//

#ifndef ENGINE_2_ITEMSEEDBLUEBERRY_H
#define ENGINE_2_ITEMSEEDBLUEBERRY_H
#include "ItemSeed.h"


class ItemSeedBlueberry : public ItemSeed {
public:
    ItemSeedBlueberry() : ItemSeed() {}
    explicit ItemSeedBlueberry(ivec2 tile);
    explicit ItemSeedBlueberry(vec2 pos);
    ~ItemSeedBlueberry() override;

    void draw(bool bind) override;
    void tick() override;
    void update(float dt) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMSEEDBLUEBERRY_H