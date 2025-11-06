//
// Created by cobble on 11/5/2025.
//

#include "ItemCarrot.h"

ItemCarrot::ItemCarrot(ivec2 tile) : Item(CARROT, tile) {
}

ItemCarrot::ItemCarrot(vec2 pos) : Item(CARROT, pos) {

}

void ItemCarrot::draw(bool bind) {
    Item::draw(bind);
}

void ItemCarrot::tick() {

}

void ItemCarrot::update(float dt) {

}

std::string ItemCarrot::getConfigKey() {
    return Item::getConfigKey() + "_CARROT";
}

std::string ItemCarrot::getConfig() {
    return Item::getConfig() + "-\n";
}

void ItemCarrot::loadConfig(const std::string &line, int i) {
    if (i == 2) {

    } else Item::loadConfig(line, i);
}
