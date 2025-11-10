//
// Created by drewg on 11/6/2025.
//

#ifndef ENGINE_2_ITEMSEED_H
#define ENGINE_2_ITEMSEED_H
#include "../Item.h"
#include "../produce/ItemProduce.h"

class ItemSeed : public Item {
public:

    class SeedData : public ItemData {
    public:
        TypeID produces = TypeID::NONE;

        SeedData() : ItemData() {}
        explicit SeedData(const std::string &configKey, const TypeID type, const TypeID parent, const TypeID produces, int textureIndex, bool isItem) : ItemData(configKey, type, parent, textureIndex, isItem) {
            this->produces = produces;
        }
        ~SeedData() override = default;
    };



    ItemSeed() : Item() {}
    explicit ItemSeed(ivec2 tile);
    explicit ItemSeed(vec2 pos);
    ~ItemSeed() override = default;
    [[nodiscard]] TypeID getType() const override;

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    void draw(float offsetX, float offsetY, bool bind) override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMSEED_H