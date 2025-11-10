//
// Created by drewg on 11/6/2025.
//

#include "ItemProduce.h"
#include "../../farmingWorld.h"

ItemProduce::ItemProduce(const ivec2 tile) : Item(tile) {

}

ItemProduce::ItemProduce(const vec2 pos) : Item(pos) {

}

FarmingObject::TypeID ItemProduce::getType() const {
    return TypeID::ITEM_PRODUCE;
}

void ItemProduce::update(const float dt) {
    Item::update(dt);
}

void ItemProduce::tick() {
    Item::tick();
}

void ItemProduce::draw(const bool bind) {
    Item::draw(bind);
}

void ItemProduce::draw(const float offsetX, const float offsetY, const bool bind) {
    Item::draw(offsetX, offsetY, bind);
}

std::string ItemProduce::getConfig() {
    return Item::getConfig() + "-\n";
}

void ItemProduce::loadConfig(const std::string &line, const int i) {
    if (i == 2) {

    } else Item::loadConfig(line, i);

}
