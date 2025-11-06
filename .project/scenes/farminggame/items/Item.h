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



    constexpr static int ITEM_TYPES = 4;
    enum ItemType {
        TOMATO_SEED = 0,
        TOMATO,
        CARROT_SEED,
        CARROT,
        NONE = 63
    };

    struct ItemData {
        ItemType category;

        ItemData(ItemType category) : category(category) {

        }
    };
    static void setTexture(const std::string &path, int width, int height, int items);
    static void loadData();





    ItemType type;
    vec2 pos;

    Item() = default;

    Item(ItemType type, ivec2 tile);
    Item(ItemType type, vec2 pos);
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