//
// Created by cobble on 11/8/2025.
//

#ifndef ENGINE_2_TASKDEPOSITITEM_H
#define ENGINE_2_TASKDEPOSITITEM_H
#include "Task.h"


class TaskDepositItem : public Task {

    FarmingObject::TypeID type = FarmingObject::TypeID::NONE;
    int amount = 0;

public:
    TaskDepositItem(LittleGuy *guy, FarmingObject::TypeID type, int amount);
    ~TaskDepositItem() override;

    bool tick() override;
    std::string getName() override;
    float getCost() override;
    void clear() override;

};


#endif //ENGINE_2_TASKDEPOSITITEM_H