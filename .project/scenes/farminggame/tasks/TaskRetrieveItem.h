//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_TASKRETRIEVEITEM_H
#define ENGINE_2_TASKRETRIEVEITEM_H
#include "Task.h"
#include "TaskHarvestPlant.h"
#include "TaskPickupItem.h"
#include "TaskTravel.h"
#include "TaskWithdrawItem.h"
#include "../items/Item.h"


class TaskRetrieveItem : public Task {

    int goalAmount = 0;
    int amount = 0;
    FarmingObject::TypeID type;
    ivec2 itemTile = ivec2(0);

    /*TaskTravel *travelTask = nullptr;
    TaskPickupItem *pickupTask = nullptr;
    TaskHarvestPlant *harvestTask = nullptr;
    TaskWithdrawItem *withdrawTask = nullptr;*/
    std::vector<Task *> tasks = std::vector<Task *>();

public:
    TaskRetrieveItem(LittleGuy *guy, FarmingObject::TypeID type, int amount);
    ~TaskRetrieveItem() override;

    bool update(float dt) override;
    bool tick() override;
    std::string getName() override;
    float getCost() override;
    void clear() override;

    bool getClosestItem();

    ivec2 getItemTile() const;
};


#endif //ENGINE_2_TASKRETRIEVEITEM_H