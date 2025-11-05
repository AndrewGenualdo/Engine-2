//
// Created by cobble on 11/5/2025.
//

#include "ItemTomatoSeed.h"

ItemTomatoSeed::ItemTomatoSeed(ivec2 tile) : Item(TOMATO_SEED, tile) {
}

ItemTomatoSeed::ItemTomatoSeed(vec2 pos) : Item(TOMATO_SEED, pos) {
}

void ItemTomatoSeed::draw(bool bind) {
    Item::draw(bind);
}

void ItemTomatoSeed::tick() {

}

void ItemTomatoSeed::update(float dt) {

}

std::string ItemTomatoSeed::getConfigKey() {
    return Item::getConfigKey() + "_TOMATO_SEED";
}

std::string ItemTomatoSeed::getConfig() {
    return Item::getConfig() + "-\n";
}

void ItemTomatoSeed::loadConfig(const std::string &line, int i) {
    if (i == 3) {

    } else Item::loadConfig(line, i);
}
