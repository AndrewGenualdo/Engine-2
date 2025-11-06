//
// Created by cobble on 11/6/2025.
//

#include "TaskPickupItem.h"
#include "../littleGuy.h"

TaskPickupItem::TaskPickupItem(LittleGuy *guy, Item::ItemType type) : Task(guy) {
    this->type = type;
}

TaskPickupItem::~TaskPickupItem() = default;

bool TaskPickupItem::update(float dt) {
    Item *closestItem = nullptr;
    float closestDist = -1;
    int closestIndex = -1;
    for (int i = 0; i < world->objects.size(); i++) {
        Item *item = dynamic_cast<Item*>(world->objects[i]);
        if (item && item->type == type) {
            float dist = length(item->pos - guy->getPos());
            if (closestItem == nullptr || closestDist > dist) {
                closestItem = item;
                closestDist = dist;
                closestIndex = i;
            }
        }
    }
    if (closestItem != nullptr && closestDist <= PICKUP_DISTANCE) {
        guy->giveItem(closestItem);
        world->objects.erase(world->objects.begin() + closestIndex);
    }
    return true;

}

std::string TaskPickupItem::getName() {
    return Task::getName();
}

float TaskPickupItem::getCost() {
    return 1;
}
