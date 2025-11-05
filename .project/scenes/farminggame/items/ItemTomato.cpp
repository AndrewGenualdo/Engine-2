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

void ItemTomato::tick() {
    Item::tick();
}

void ItemTomato::update(float dt) {

}

std::string ItemTomato::getConfigKey() {
    return Item::getConfigKey() + "_TOMATO";
}

std::string ItemTomato::getConfig() {
    return Item::getConfig() + "-\n";
}

void ItemTomato::loadConfig(const std::string &line, int i) {
    if (i == 2) {

    } else Item::loadConfig(line, i);

}
