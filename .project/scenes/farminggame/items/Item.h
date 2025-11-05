//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEM_H
#define ENGINE_2_ITEM_H

#include <glm/vec2.hpp>
#include <cobb/misc/multiTexture2d.h>

using namespace glm;
using namespace cobb;

class Item {

    static MultiTexture2d itemsTexture;
    static int width, height;

public:

    static void setTexture(const std::string &path, int width, int height, int items);

    enum ItemType {
        TOMATO_SEED = 0,
        TOMATO,
        NONE = 63
    };

    ItemType type;
    ivec2 tile;
    vec2 pos;

    Item(ItemType type, ivec2 tile);
    Item(ItemType type, vec2 pos);
    virtual ~Item() = default;

    virtual void update(float dt);
    virtual void draw(bool bind);

};


#endif //ENGINE_2_ITEM_H