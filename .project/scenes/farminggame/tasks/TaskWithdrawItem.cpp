//
// Created by cobble on 11/8/2025.
//

#include "TaskWithdrawItem.h"
#include "../misc/LittleGuy.h"

TaskWithdrawItem::TaskWithdrawItem(LittleGuy *guy, FarmingObject::TypeID type, int amount) {
    this->guy = guy;
    this->type = type;
    this->amount = amount;
}

TaskWithdrawItem::~TaskWithdrawItem() = default;

bool TaskWithdrawItem::tick() {
    if (guy->tile == FarmingWorld::INVENTORY_TILE) {
        int itemAmt = world->inventory[type];
        if (itemAmt > 0) {
            world->inventory[type]--;
            guy->giveItem(type);
            amount--;
        } else return true;
    } else return true;
    return amount <= 0;
}

std::string TaskWithdrawItem::getName() {
    return "Withdraw Item: { " + FarmingObject::getData<FarmingObject::ObjectData>(type)->configKey + ": " + std::to_string(this->amount) + " } " ;
}

float TaskWithdrawItem::getCost() {
    return static_cast<float>(amount);
}

int TaskWithdrawItem::getAmount() const {
    return amount;
}
