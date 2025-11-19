//
// Created by cobble on 11/19/2025.
//

#include "ItemSeedBlueberry.h"

ItemSeedBlueberry::ItemSeedBlueberry(ivec2 tile) : ItemSeed(tile) {
}

ItemSeedBlueberry::ItemSeedBlueberry(vec2 pos) : ItemSeed(pos) {
}

ItemSeedBlueberry::~ItemSeedBlueberry() {
}

void ItemSeedBlueberry::draw(bool bind) {
    ItemSeed::draw(bind);
}

void ItemSeedBlueberry::tick() {

}

void ItemSeedBlueberry::update(float dt) {
    ItemSeed::update(dt);
}

FarmingObject::TypeID ItemSeedBlueberry::getType() const {
    return TypeID::ITEM_SEED_BLUEBERRY;
}

std::string ItemSeedBlueberry::getConfig() {
    return ItemSeed::getConfig() + "-\n";
}

void ItemSeedBlueberry::loadConfig(const std::string &line, int i) {
    if (i == 3) {

    } else ItemSeed::loadConfig(line, i);
}
