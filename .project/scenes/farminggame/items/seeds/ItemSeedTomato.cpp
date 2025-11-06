//
// Created by cobble on 11/5/2025.
//

#include "ItemSeedTomato.h"

ItemSeedTomato::ItemSeedTomato(const ivec2 tile) : ItemSeed(TOMATO, tile) {

}

ItemSeedTomato::ItemSeedTomato(const vec2 pos) : ItemSeed(TOMATO, pos) {

}

void ItemSeedTomato::draw(const bool bind) {
    ItemSeed::draw(bind);
}

void ItemSeedTomato::tick() {

}

void ItemSeedTomato::update(float dt) {

}

std::string ItemSeedTomato::getConfigKey() {
    return ItemSeed::getConfigKey() + "_TOMATO";
}

std::string ItemSeedTomato::getConfig() {
    return ItemSeed::getConfig() + "-\n";
}

void ItemSeedTomato::loadConfig(const std::string &line, const int i) {
    if (i == 3) {

    } else ItemSeed::loadConfig(line, i);
}
