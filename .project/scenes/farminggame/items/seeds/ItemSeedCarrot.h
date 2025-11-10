//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEMSEEDCARROT_H
#define ENGINE_2_ITEMSEEDCARROT_H
#include "ItemSeed.h"


class ItemSeedCarrot : public ItemSeed {

public:
    ItemSeedCarrot() : ItemSeed() {}
    explicit ItemSeedCarrot(ivec2 tile);
    explicit ItemSeedCarrot(vec2 pos);
    ~ItemSeedCarrot() override;

    void draw(bool bind) override;
    void tick() override;
    void update(float dt) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};
#endif //ENGINE_2_ITEMSEEDCARROT_H
