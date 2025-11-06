//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEMPRODUCECARROT_H
#define ENGINE_2_ITEMPRODUCECARROT_H
#include "ItemProduce.h"


class ItemProduceCarrot  : public ItemProduce {

public:
    ItemProduceCarrot() = default;
    explicit ItemProduceCarrot(ivec2 tile);
    explicit ItemProduceCarrot(vec2 pos);

    void draw(bool bind) override;
    void tick() override;
    void update(float dt) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};


#endif //ENGINE_2_ITEMPRODUCECARROT_H