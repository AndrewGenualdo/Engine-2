//
// Created by cobble on 11/5/2025.
//

#include "TaskRetrieveItem.h"
#include "../items/Item.h"
#include "../littleGuy.h"

TaskRetrieveItem::TaskRetrieveItem(LittleGuy *guy, Item::ItemType type, int amount) : Task(guy) {
    this->goalAmount = amount;
    this->type = type;
    getClosestItem();
}

TaskRetrieveItem::~TaskRetrieveItem() {
    if (travelTask != nullptr) {
        delete travelTask;
        travelTask = nullptr;
    }
}

bool TaskRetrieveItem::update(float dt) {
    if (travelTask != nullptr) {
        if (amount < goalAmount) {
            bool res = travelTask->update(dt);
            if (res) {
                delete travelTask;
                travelTask = nullptr;
                pickupTask = new TaskPickupItem(guy, type);
            }
            return false;
        }
    } else if (pickupTask != nullptr) {
        bool res = pickupTask->update(dt);
        if (res) {
            delete pickupTask;
            pickupTask = nullptr;
            amount++;
            getClosestItem();
        }
        return false;
    }

    return true;
}

std::string TaskRetrieveItem::getName() {
    return Task::getName();
}

float TaskRetrieveItem::getCost() {
    return Task::getCost();
}

bool TaskRetrieveItem::getClosestItem() {



    //if pickup item

    Item *closestItem = nullptr;
    for (auto & object : world->objects) {
        Item *item = dynamic_cast<Item*>(object);
        if (item && item->type == type) {
            if (closestItem == nullptr || length(closestItem->pos - guy->getPos()) > length(item->pos - guy->getPos())) {
                closestItem = item;
            }
        }
    }
    if (closestItem != nullptr) {
        travelTask = new TaskTravel(guy, closestItem->tile);
    }
    return closestItem == nullptr;
}
