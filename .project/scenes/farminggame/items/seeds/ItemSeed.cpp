//
// Created by drewg on 11/6/2025.
//

#include "ItemSeed.h"
#include "../../FarmingWorld.h"

ItemSeed::ItemSeed(const ivec2 tile) : Item(tile) {

}

ItemSeed::ItemSeed(const vec2 pos) : Item(pos) {

}

FarmingObject::TypeID ItemSeed::getType() const {
    return TypeID::ITEM_SEED;
}

void ItemSeed::update(const float dt) {
    Item::update(dt);
}

void ItemSeed::tick() {
    Item::tick();
}

void ItemSeed::draw(const bool bind) {
    Item::draw(bind);
}

void ItemSeed::draw(const float offsetX, const float offsetY, const bool bind) {
    Item::draw(offsetX, offsetY, bind);
}

std::string ItemSeed::getConfig() {
    return Item::getConfig() + "-\n";
}

void ItemSeed::loadConfig(const std::string &line, const int i) {
    if (i == 2) {

    } else Item::loadConfig(line, i);

}
