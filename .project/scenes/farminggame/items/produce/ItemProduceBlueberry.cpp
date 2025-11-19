//
// Created by cobble on 11/19/2025.
//

#include "ItemProduceBlueberry.h"

ItemProduceBlueberry::ItemProduceBlueberry(ivec2 tile) : ItemProduce(tile) {
}

ItemProduceBlueberry::ItemProduceBlueberry(vec2 pos) : ItemProduce(pos) {
}

void ItemProduceBlueberry::update(float dt) {

}

void ItemProduceBlueberry::tick() {

}

void ItemProduceBlueberry::draw(bool bind) {
    ItemProduce::draw(bind);
}

FarmingObject::TypeID ItemProduceBlueberry::getType() const {
    return TypeID::ITEM_PRODUCE_BLUEBERRY;
}

std::string ItemProduceBlueberry::getConfig() {
    return ItemProduce::getConfig() + "-\n";
}

void ItemProduceBlueberry::loadConfig(const std::string &line, int i) {
    if (i == 3) {

    } else ItemProduce::loadConfig(line, i);
}
