//
// Created by cobble on 11/5/2025.
//

#include "ItemTomato.h"
#include "../../farmingWorld.h"


ItemTomato::ItemTomato(const ivec2 tile) : ItemProduce(TOMATO, tile) {

}

ItemTomato::ItemTomato(const vec2 pos) : ItemProduce(TOMATO, pos) {

}

void ItemTomato::draw(const bool bind) {
    ItemProduce::draw(bind);
}

void ItemTomato::tick() {
    ItemProduce::tick();
}

void ItemTomato::update(const float dt) {

}

std::string ItemTomato::getConfigKey() {
    return ItemProduce::getConfigKey() + "_TOMATO";
}

std::string ItemTomato::getConfig() {
    return ItemProduce::getConfig() + "-\n";
}

void ItemTomato::loadConfig(const std::string &line, const int i) {
    if (i == 2) {

    } else ItemProduce::loadConfig(line, i);

}
