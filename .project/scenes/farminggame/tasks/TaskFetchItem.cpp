//
// Created by cobble on 11/7/2025.
//

#include "TaskFetchItem.h"

TaskFetchItem::TaskFetchItem(LittleGuy *guy, FarmingObject::TypeID type, int amount, ivec2 returnToTile) {
    this->guy = guy;
    this->returnToTile = returnToTile;
    this->taskRetrieveItem = new TaskRetrieveItem(guy, type, amount);
    this->taskTravel = nullptr;
    if (returnToTile == FarmingWorld::INVENTORY_TILE) taskDepositItem = new TaskDepositItem(guy, type, amount);
    else this->taskDepositItem = nullptr;

}

TaskFetchItem::~TaskFetchItem() {
    if (taskRetrieveItem != nullptr) {
        delete taskRetrieveItem;
        taskRetrieveItem = nullptr;
    }
    if (taskTravel != nullptr) {
        delete taskTravel;
        taskTravel = nullptr;
    }
    if (taskDepositItem != nullptr) {
        delete taskDepositItem;
        taskDepositItem = nullptr;
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
    if (taskDepositItem != nullptr) {
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
    if (taskDepositItem != nullptr) {
        if (taskDepositItem->tick()) {
            delete taskDepositItem;
            taskDepositItem = nullptr;
        }
        return false;
    }
    return true;
}

std::string TaskFetchItem::getName() {
    std::string out = "Fetch Item: {";
    if (taskRetrieveItem != nullptr) out += taskRetrieveItem->getName();
    if (taskTravel != nullptr) out += taskTravel->getName();
    if (taskDepositItem != nullptr) out += taskDepositItem->getName();
    out += "} ";
    return out;
}

float TaskFetchItem::getCost() {
    return Task::getCost();
}

void TaskFetchItem::setActive(bool active) {
    if (taskRetrieveItem != nullptr) taskRetrieveItem->setActive(active);
    if (taskTravel != nullptr) taskTravel->setActive(active);
    if (taskDepositItem != nullptr) taskDepositItem->setActive(active);
}
