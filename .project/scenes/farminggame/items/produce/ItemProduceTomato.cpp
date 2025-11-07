//
// Created by cobble on 11/5/2025.
//

#include "ItemProduceTomato.h"
#include "../../farmingWorld.h"


ItemProduceTomato::ItemProduceTomato(const ivec2 tile) : ItemProduce(tile) {

}

ItemProduceTomato::ItemProduceTomato(const vec2 pos) : ItemProduce(pos) {

}

void ItemProduceTomato::draw(const bool bind) {
    ItemProduce::draw(bind);
}

FarmingObject::TypeID ItemProduceTomato::getType() const {
    return TypeID::ITEM_PRODUCE_TOMATO;
}

void ItemProduceTomato::tick() {
    ItemProduce::tick();
}

void ItemProduceTomato::update(const float dt) {

}

std::string ItemProduceTomato::getConfigKey() {
    return ItemProduce::getConfigKey() + "_TOMATO";
}

std::string ItemProduceTomato::getConfig() {
    return ItemProduce::getConfig() + "-\n";
}

void ItemProduceTomato::loadConfig(const std::string &line, const int i) {
    if (i == 3) {

    } else ItemProduce::loadConfig(line, i);

}
