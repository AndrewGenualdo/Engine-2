//
// Created by cobble on 11/5/2025.
//

#include "ItemCarrotSeed.h"

ItemCarrotSeed::ItemCarrotSeed(const ivec2 tile) : ItemSeed(CARROT, tile) {

}

ItemCarrotSeed::ItemCarrotSeed(const vec2 pos) : ItemSeed(CARROT, pos) {

}

ItemCarrotSeed::~ItemCarrotSeed() = default;

void ItemCarrotSeed::draw(const bool bind) {
    ItemSeed::draw(bind);
}

void ItemCarrotSeed::tick() {

}

void ItemCarrotSeed::update(const float dt) {

}

std::string ItemCarrotSeed::getConfigKey() {
    return ItemSeed::getConfigKey() + "_CARROT";
}

std::string ItemCarrotSeed::getConfig() {
    return ItemSeed::getConfig() + "-\n";
}

void ItemCarrotSeed::loadConfig(const std::string &line, const int i) {
    if (i == 3) {

    } else ItemSeed::loadConfig(line, i);
}
