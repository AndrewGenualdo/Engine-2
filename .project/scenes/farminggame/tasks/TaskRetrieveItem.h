//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_TASKRETRIEVEITEM_H
#define ENGINE_2_TASKRETRIEVEITEM_H
#include "Task.h"
#include "TaskHarvestPlant.h"
#include "TaskPickupItem.h"
#include "TaskTravel.h"
#include "../items/Item.h"


class Item;

class TaskRetrieveItem : public Task {

    int goalAmount = 0;
    int amount = 0;
    FarmingObject::TypeID type;

    TaskTravel *travelTask = nullptr;
    TaskPickupItem *pickupTask = nullptr;
    TaskHarvestPlant *harvestTask = nullptr;

public:
    TaskRetrieveItem(LittleGuy *guy, FarmingObject::TypeID type, int amount);
    ~TaskRetrieveItem() override;

    bool update(float dt) override;
    std::string getName() override;
    float getCost() override;

    bool getClosestItem();
};


#endif //ENGINE_2_TASKRETRIEVEITEM_H