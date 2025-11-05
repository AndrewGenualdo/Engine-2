//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEMTOMATO_H
#define ENGINE_2_ITEMTOMATO_H


#include "Item.h"


class ItemTomato : public Item {


    explicit ItemTomato(ivec2 tile);
    explicit ItemTomato(vec2 pos);

    void draw(bool bind) override;
    void update(float dt) override;

};


#endif //ENGINE_2_ITEMTOMATO_H