//
// Created by cobble on 11/5/2025.
//

#include "ItemProduceCarrot.h"

ItemProduceCarrot::ItemProduceCarrot(const ivec2 tile) : ItemProduce(CARROT, tile) {
}

ItemProduceCarrot::ItemProduceCarrot(const vec2 pos) : ItemProduce(CARROT, pos) {

}

void ItemProduceCarrot::draw(const bool bind) {
    ItemProduce::draw(bind);
}

void ItemProduceCarrot::tick() {

}

void ItemProduceCarrot::update(float dt) {

}

std::string ItemProduceCarrot::getConfigKey() {
    return ItemProduce::getConfigKey() + "_CARROT";
}

std::string ItemProduceCarrot::getConfig() {
    return ItemProduce::getConfig() + "-\n";
}

void ItemProduceCarrot::loadConfig(const std::string &line, int i) {
    if (i == 3) {

    } else ItemProduce::loadConfig(line, i);
}
