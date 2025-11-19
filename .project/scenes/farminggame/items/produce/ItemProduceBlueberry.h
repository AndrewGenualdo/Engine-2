//
// Created by cobble on 11/19/2025.
//

#ifndef ENGINE_2_ITEMPRODUCEBLUEBERRY_H
#define ENGINE_2_ITEMPRODUCEBLUEBERRY_H
#include "ItemProduce.h"


class ItemProduceBlueberry : public ItemProduce {
public:
    ItemProduceBlueberry() = default;
    explicit ItemProduceBlueberry(ivec2 tile);
    explicit ItemProduceBlueberry(vec2 pos);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMPRODUCEBLUEBERRY_H