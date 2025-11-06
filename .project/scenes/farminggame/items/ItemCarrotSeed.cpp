//
// Created by cobble on 11/5/2025.
//

#include "ItemCarrotSeed.h"

ItemCarrotSeed::ItemCarrotSeed(ivec2 tile) : Item(CARROT_SEED, tile) {

}

ItemCarrotSeed::ItemCarrotSeed(vec2 pos) : Item(CARROT_SEED, pos) {

}

void ItemCarrotSeed::draw(bool bind) {
    Item::draw(bind);
}

void ItemCarrotSeed::tick() {

}

void ItemCarrotSeed::update(float dt) {

}

std::string ItemCarrotSeed::getConfigKey() {
    return Item::getConfigKey() + "_CARROT_SEED";
}

std::string ItemCarrotSeed::getConfig() {
    return Item::getConfig() + "-\n";
}

void ItemCarrotSeed::loadConfig(const std::string &line, int i) {
    if (i == 2) {

    } else Item::loadConfig(line, i);
}
