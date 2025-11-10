//
// Created by cobble on 11/6/2025.
//

#ifndef ENGINE_2_TASKPICKUPITEM_H
#define ENGINE_2_TASKPICKUPITEM_H
#include "Task.h"
#include "../items/Item.h"

class TaskPickupItem : public Task {
    constexpr static float PICKUP_DISTANCE = (FarmingWorld::TILE_WIDTH + FarmingWorld::TILE_HEIGHT) * 0.5f;

    Item *item;

public:
    TaskPickupItem(LittleGuy *guy, Item *item);
    ~TaskPickupItem() override;

    bool tick() override;
    std::string getName() override;
    float getCost() override;
    void setActive(bool active) override;

    void setItem(Item *item);
};


#endif //ENGINE_2_TASKPICKUPITEM_H