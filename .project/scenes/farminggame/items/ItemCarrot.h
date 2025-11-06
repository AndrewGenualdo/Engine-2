//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEMCARROT_H
#define ENGINE_2_ITEMCARROT_H
#include "Item.h"


class ItemCarrot  : public Item {

public:
    ItemCarrot() = default;
    explicit ItemCarrot(ivec2 tile);
    explicit ItemCarrot(vec2 pos);

    void draw(bool bind) override;
    void tick() override;
    void update(float dt) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};


#endif //ENGINE_2_ITEMCARROT_H