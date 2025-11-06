//
// Created by cobble on 11/5/2025.
//

#include "ItemTomatoSeed.h"

ItemTomatoSeed::ItemTomatoSeed(const ivec2 tile) : ItemSeed(TOMATO, tile) {

}

ItemTomatoSeed::ItemTomatoSeed(const vec2 pos) : ItemSeed(TOMATO, pos) {

}

void ItemTomatoSeed::draw(const bool bind) {
    ItemSeed::draw(bind);
}

void ItemTomatoSeed::tick() {

}

void ItemTomatoSeed::update(float dt) {

}

std::string ItemTomatoSeed::getConfigKey() {
    return ItemSeed::getConfigKey() + "_TOMATO";
}

std::string ItemTomatoSeed::getConfig() {
    return ItemSeed::getConfig() + "-\n";
}

void ItemTomatoSeed::loadConfig(const std::string &line, const int i) {
    if (i == 3) {

    } else ItemSeed::loadConfig(line, i);
}
