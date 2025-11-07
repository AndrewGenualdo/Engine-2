//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEMPRODUCETOMATO_H
#define ENGINE_2_ITEMPRODUCETOMATO_H


#include "ItemProduce.h"


class ItemProduceTomato : public ItemProduce {

public:
    ItemProduceTomato() = default;
    explicit ItemProduceTomato(ivec2 tile);
    explicit ItemProduceTomato(vec2 pos);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};


#endif //ENGINE_2_ITEMPRODUCETOMATO_H