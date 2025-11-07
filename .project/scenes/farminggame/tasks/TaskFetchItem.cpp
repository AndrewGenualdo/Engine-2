//
// Created by cobble on 11/7/2025.
//

#include "TaskFetchItem.h"

TaskFetchItem::TaskFetchItem(LittleGuy *guy, FarmingObject::TypeID type, int amount, ivec2 returnToTile) {
    this->guy = guy;
    this->returnToTile = returnToTile;
    this->taskRetrieveItem = new TaskRetrieveItem(guy, type, amount);
    this->taskTravel = nullptr;
}

TaskFetchItem::~TaskFetchItem() {
    if (taskRetrieveItem != nullptr) {
        delete taskRetrieveItem;
        taskRetrieveItem = nullptr;
    }
    if (taskTravel != nullptr) {
        taskTravel = nullptr;
    }
}

bool TaskFetchItem::update(float dt) {
    if (taskRetrieveItem != nullptr) {
        if (taskRetrieveItem->update(dt)) {
            delete taskRetrieveItem;
            taskRetrieveItem = nullptr;
            taskTravel = new TaskTravel(guy, returnToTile);
        }
        return false;
    }
    if (taskTravel != nullptr) {
        if (taskTravel->update(dt)) {
            delete taskTravel;
            taskTravel = nullptr;
        }
        return false;
    }
    return true;
}

bool TaskFetchItem::tick() {
    if (taskRetrieveItem != nullptr) {
        if (taskRetrieveItem->tick()) {
            delete taskRetrieveItem;
            taskRetrieveItem = nullptr;
            taskTravel = new TaskTravel(guy, returnToTile);
        }
        return false;
    }
    if (taskTravel != nullptr) {
        return false;
    }

    return true;
}

std::string TaskFetchItem::getName() {
    return "Fetch Item";
}

float TaskFetchItem::getCost() {
    return Task::getCost();
}
