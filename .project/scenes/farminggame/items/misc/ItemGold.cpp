//
// Created by andrew.genualdo on 11/21/2025.
//

#include "ItemGold.h"

ItemGold::ItemGold(const ivec2 tile) : Item(tile) {
}

ItemGold::ItemGold(const vec2 pos) : Item(pos){
}

FarmingObject::TypeID ItemGold::getType() const {
    return TypeID::ITEM_GOLD;
}

std::string ItemGold::getConfig() {
    return "ITEM_GOLD";
}

void ItemGold::loadConfig(const std::string &line, const int i) {
    if (i == 2) {

    } else Item::loadConfig(line, i);
}
