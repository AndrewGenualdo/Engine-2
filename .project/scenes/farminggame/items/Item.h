//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEM_H
#define ENGINE_2_ITEM_H

#include "../FarmingObject.h"

using namespace glm;
using namespace cobb;

class Item : public FarmingObject {

    static MultiTexture2d itemsTexture;
    static int width, height;

public:

    static void setTexture(const std::string &path, int width, int height, int items);

    enum ItemType {
        NONE,
        SEED,
        PRODUCE
    };

    class ItemData : public ObjectData {
    public:
        ItemType itemType = NONE;

        ItemData() : ObjectData() {}
        explicit ItemData(const ItemType itemType) : ObjectData(ITEM) {
            this->itemType = itemType;
        };
    };




    vec2 pos;
    ItemType itemType = NONE;

    Item() = default;

    Item(ItemType itemType, ivec2 tile);
    Item(ItemType itemType, vec2 pos);
    ~Item() override = default;

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    void draw(float offsetX, float offsetY, bool bind) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};


#endif //ENGINE_2_ITEM_H