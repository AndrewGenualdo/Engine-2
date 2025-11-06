//
// Created by drewg on 11/6/2025.
//

#ifndef ENGINE_2_ITEMPRODUCE_H
#define ENGINE_2_ITEMPRODUCE_H

#include "../Item.h"

class ItemProduce : public Item {

public:
    enum ProduceType {
        NONE,
        TOMATO,
        CARROT,
    };

    class ProduceData : public ItemData {
    public:
        ProduceType produceType = NONE;

        ProduceData() : ItemData() {}
        explicit ProduceData(const ProduceType produceType) : ItemData(PRODUCE) {
            this->produceType = produceType;
        }
    };

    ProduceType produceType = NONE;

    ItemProduce() : Item() {}
    ItemProduce(ProduceType produceType, ivec2 tile);
    ItemProduce(ProduceType produceType, vec2 pos);
    ~ItemProduce() override = default;

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    void draw(float offsetX, float offsetY, bool bind) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMPRODUCE_H