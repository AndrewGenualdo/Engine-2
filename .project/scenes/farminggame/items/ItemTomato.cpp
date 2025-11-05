//
// Created by cobble on 11/5/2025.
//

#include "ItemTomato.h"
#include "../farmingWorld.h"


ItemTomato::ItemTomato(ivec2 tile) : Item(TOMATO, tile) {

}

ItemTomato::ItemTomato(vec2 pos) : Item(TOMATO, pos) {

}

void ItemTomato::draw(bool bind) {
    Item::draw(bind);
}

void ItemTomato::update(float dt) {

}
