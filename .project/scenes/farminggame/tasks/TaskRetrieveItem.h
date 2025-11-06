//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_TASKRETRIEVEITEM_H
#define ENGINE_2_TASKRETRIEVEITEM_H
#include "Task.h"
#include "TaskPickupItem.h"
#include "TaskTravel.h"
#include "../items/Item.h"


class Item;

class TaskRetrieveItem : public Task {

    int goalAmount = 0;
    int amount = 0;
    Item::ItemType type;

    TaskTravel *travelTask = nullptr;
    TaskPickupItem *pickupTask = nullptr;

public:
    TaskRetrieveItem(LittleGuy *guy, Item::ItemType type, int amount);
    ~TaskRetrieveItem() override;

    bool update(float dt) override;
    std::string getName() override;
    float getCost() override;

    bool getClosestItem();
};


#endif //ENGINE_2_TASKRETRIEVEITEM_H