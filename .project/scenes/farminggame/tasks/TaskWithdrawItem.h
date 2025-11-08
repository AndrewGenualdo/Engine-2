//
// Created by cobble on 11/8/2025.
//

#ifndef ENGINE_2_TASKWITHDRAWITEM_H
#define ENGINE_2_TASKWITHDRAWITEM_H
#include "Task.h"


class TaskWithdrawItem : public Task {
    FarmingObject::TypeID type = FarmingObject::TypeID::NONE;
    int amount = 0;

public:
    TaskWithdrawItem(LittleGuy *guy, FarmingObject::TypeID type, int amount);
    ~TaskWithdrawItem() override;

    bool tick() override;
    std::string getName() override;
    float getCost() override;
    void clear() override;
};


#endif //ENGINE_2_TASKWITHDRAWITEM_H