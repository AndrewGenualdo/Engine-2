//
// Created by cobble on 11/6/2025.
//

#include "TaskPickupItem.h"
#include "../littleGuy.h"

TaskPickupItem::TaskPickupItem(LittleGuy *guy, Item *item) : Task(guy) {
    this->item = item;
    if (this->item != nullptr) this->item->setBeingUsed(true);
}

TaskPickupItem::~TaskPickupItem() = default;

bool TaskPickupItem::tick() {
    guy->giveItem(item);
    for (int i = world->objects.size() - 1; i >= 0; i--) {
        if (world->objects[i] == item) {
            world->objects.erase(world->objects.begin() + i);
            break;
        }
    }
    return true;
}

std::string TaskPickupItem::getName() {
    return "Pickup Item";
}

float TaskPickupItem::getCost() {
    return 1;
}

void TaskPickupItem::setItem(Item *item) {
    if (this->item != nullptr) this->item->setBeingUsed(false);
    this->item = item;
    this->item->setBeingUsed(true);
}
