//
// Created by cobble on 11/5/2025.
//

#include "ItemSeedCarrot.h"

ItemSeedCarrot::ItemSeedCarrot(const ivec2 tile) : ItemSeed(tile) {

}

ItemSeedCarrot::ItemSeedCarrot(const vec2 pos) : ItemSeed(pos) {

}

ItemSeedCarrot::~ItemSeedCarrot() = default;

void ItemSeedCarrot::draw(const bool bind) {
    ItemSeed::draw(bind);
}

void ItemSeedCarrot::tick() {

}

void ItemSeedCarrot::update(const float dt) {

}

FarmingObject::TypeID ItemSeedCarrot::getType() const {
    return TypeID::ITEM_SEED_CARROT;
}

std::string ItemSeedCarrot::getConfig() {
    return ItemSeed::getConfig() + "-\n";
}

void ItemSeedCarrot::loadConfig(const std::string &line, const int i) {
    if (i == 3) {

    } else ItemSeed::loadConfig(line, i);
}
