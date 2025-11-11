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
    if (item != nullptr) {
        guy->giveItem(item->getType());
        for (int i = world->items.size() - 1; i >= 0; i--) {
            if (world->items[i] == item) {
                delete world->items[i];
                world->items[i] = nullptr;
                world->items.erase(world->items.begin() + i);
                break;
            }
        }
    }
    return true;
}

std::string TaskPickupItem::getName() {
    std::string out = "Pickup Item: {";
    if (item == nullptr) out += "NULL";
    else out += std::to_string(item->tile.x ) + ", " + std::to_string(item->tile.y) + " ; " + std::to_string(static_cast<int>(item->getType()));
    out +="} ";
    return out;
}

float TaskPickupItem::getCost() {
    return 1;
}

void TaskPickupItem::setActive(bool active) {
    if (item != nullptr) item->setBeingUsed(active);
}

void TaskPickupItem::setItem(Item *item) {
    if (this->item != nullptr) this->item->setBeingUsed(true);
    this->item = item;
}
