//
// Created by drewg on 11/6/2025.
//

#ifndef ENGINE_2_ITEMPRODUCE_H
#define ENGINE_2_ITEMPRODUCE_H

#include "../Item.h"
#include "../../tiles/plants/TilePlant.h"

class ItemProduce : public Item {

public:
    class ProduceData : public ItemData {
    public:
        TypeID producedFrom = TypeID::NONE;

        ProduceData() : ItemData() {}
        explicit ProduceData(const std::string &configKey, const TypeID type, const TypeID parent, const TypeID producedFrom, int textureIndex, bool isItem) : ItemData(configKey, type, parent, textureIndex, isItem) {
            this->producedFrom = producedFrom;
        }
        ~ProduceData() override = default;

    };



    ItemProduce() : Item() {}
    explicit ItemProduce(ivec2 tile);
    explicit ItemProduce(vec2 pos);
    ~ItemProduce() override = default;
    [[nodiscard]] TypeID getType() const override;

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    void draw(float offsetX, float offsetY, bool bind) override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMPRODUCE_H