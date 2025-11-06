//
// Created by cobble on 11/6/2025.
//

#ifndef ENGINE_2_TASKPICKUPITEM_H
#define ENGINE_2_TASKPICKUPITEM_H
#include "Task.h"
#include "../items/Item.h"

class TaskPickupItem : Task {
    constexpr static float PICKUP_DISTANCE = (FarmingWorld::TILE_WIDTH + FarmingWorld::TILE_HEIGHT) * 0.5f;

    Item::ItemType type;

public:
    TaskPickupItem(LittleGuy *guy, Item::ItemType type);
    ~TaskPickupItem() override;

    bool update(float dt) override;
    std::string getName() override;
    float getCost() override;
};


#endif //ENGINE_2_TASKPICKUPITEM_H