//
// Created by cobble on 11/5/2025.
//

#include "ItemProduceCarrot.h"

ItemProduceCarrot::ItemProduceCarrot(const ivec2 tile) : ItemProduce(tile) {
}

ItemProduceCarrot::ItemProduceCarrot(const vec2 pos) : ItemProduce(pos) {

}

void ItemProduceCarrot::draw(const bool bind) {
    ItemProduce::draw(bind);
}

FarmingObject::TypeID ItemProduceCarrot::getType() const {
    return TypeID::ITEM_PRODUCE_CARROT;
}

void ItemProduceCarrot::tick() {

}

void ItemProduceCarrot::update(float dt) {

}

std::string ItemProduceCarrot::getConfig() {
    return ItemProduce::getConfig() + "-\n";
}

void ItemProduceCarrot::loadConfig(const std::string &line, int i) {
    if (i == 3) {

    } else ItemProduce::loadConfig(line, i);
}
