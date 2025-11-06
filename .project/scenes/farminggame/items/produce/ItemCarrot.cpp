//
// Created by cobble on 11/5/2025.
//

#include "ItemCarrot.h"

ItemCarrot::ItemCarrot(const ivec2 tile) : ItemProduce(CARROT, tile) {
}

ItemCarrot::ItemCarrot(const vec2 pos) : ItemProduce(CARROT, pos) {

}

void ItemCarrot::draw(const bool bind) {
    ItemProduce::draw(bind);
}

void ItemCarrot::tick() {

}

void ItemCarrot::update(float dt) {

}

std::string ItemCarrot::getConfigKey() {
    return ItemProduce::getConfigKey() + "_CARROT";
}

std::string ItemCarrot::getConfig() {
    return ItemProduce::getConfig() + "-\n";
}

void ItemCarrot::loadConfig(const std::string &line, int i) {
    if (i == 3) {

    } else ItemProduce::loadConfig(line, i);
}
