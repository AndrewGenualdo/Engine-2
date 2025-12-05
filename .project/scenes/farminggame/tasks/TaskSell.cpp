//
// Created by andrew.genualdo on 12/5/2025.
//

#include "TaskSell.h"
#include "../misc/LittleGuy.h"
TaskSell::TaskSell(LittleGuy *guy, const FarmingObject::TypeID type, const int amount) {
    this->guy = guy;
    this->type = type;
    this->amount = amount;
}

TaskSell::~TaskSell() = default;

bool TaskSell::tick() {
    if (guy->tile == FarmingWorld::STAND_TILE) {
        Item *item = guy->takeItem(type);
        if (item != nullptr) {
            delete item;
            amount--;
            //world->inventory[FarmingObject::TypeID::ITEM_GOLD]++;
            //world->effectiveInventory[FarmingObject::TypeID::ITEM_GOLD]++;
            guy->giveItem(FarmingObject::TypeID::ITEM_GOLD);
        } else return true;
    } else return true;
    return amount <= 0;
}

std::string TaskSell::getName() {
    return "Sell Item: { " + FarmingObject::getData<FarmingObject::ObjectData>(this->type)->configKey + " } ";
}

float TaskSell::getCost() {
    return static_cast<float>(amount);
}
