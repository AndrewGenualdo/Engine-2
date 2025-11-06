//
// Created by drewg on 11/6/2025.
//

#ifndef ENGINE_2_ITEMSEED_H
#define ENGINE_2_ITEMSEED_H
#include "../Item.h"
#include "../produce/ItemProduce.h"

class ItemSeed : public Item {
public:
    enum SeedType {
        NONE,
        TOMATO,
        CARROT,
    };

    class SeedData : public ItemData {
    public:
        SeedType seedType = NONE;
        ItemProduce::ProduceType produces = ItemProduce::ProduceType::NONE;

        SeedData() : ItemData() {}
        explicit SeedData(const SeedType seedType, const ItemProduce::ProduceType produces) : ItemData(SEED) {
            this->seedType = seedType;
            this->produces = produces;
        }
    };

    SeedType seedType = NONE;

    ItemSeed() : Item() {}
    ItemSeed(SeedType seedType, ivec2 tile);
    ItemSeed(SeedType seedType, vec2 pos);
    ~ItemSeed() override = default;

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    void draw(float offsetX, float offsetY, bool bind) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMSEED_H