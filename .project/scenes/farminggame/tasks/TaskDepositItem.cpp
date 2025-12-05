//
// Created by cobble on 11/8/2025.
//

#include "TaskDepositItem.h"

#include "../misc/LittleGuy.h"

TaskDepositItem::TaskDepositItem(LittleGuy *guy, const FarmingObject::TypeID type, const int amount) {
    this->guy = guy;
    this->type = type;
    this->amount = amount;
}

TaskDepositItem::~TaskDepositItem()  = default;

bool TaskDepositItem::tick() {
    if (guy->tile == FarmingWorld::INVENTORY_TILE) {
        Item *item = guy->takeItem(type);
        if (item != nullptr) {
            delete item;
            world->inventory[type]++;
            world->effectiveInventory[type]++;
            amount--;
        } else return true;
    } else return true;
    return amount <= 0;
}

std::string TaskDepositItem::getName() {
    return "Deposit Item: { " + FarmingObject::getData<FarmingObject::ObjectData>(this->type)->configKey + " } ";
}

float TaskDepositItem::getCost() {
    return static_cast<float>(amount);
}
