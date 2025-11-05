//
// Created by cobble on 11/5/2025.
//

#include "TaskRetrieveItem.h"
#include "../items/Item.h"

TaskRetrieveItem::TaskRetrieveItem(LittleGuy *guy, Item item, int amount) {
}

bool TaskRetrieveItem::update(float dt) {
    return Task::update(dt);
}

std::string TaskRetrieveItem::getName() {
    return Task::getName();
}
