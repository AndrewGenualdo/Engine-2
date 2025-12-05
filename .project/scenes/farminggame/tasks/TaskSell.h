//
// Created by andrew.genualdo on 12/5/2025.
//

#ifndef ENGINE_2_TASKSELL_H
#define ENGINE_2_TASKSELL_H
#include "Task.h"


class TaskSell : public Task {

    FarmingObject::TypeID type = FarmingObject::TypeID::NONE;
    int amount = 0;

public:
    TaskSell(LittleGuy *guy, FarmingObject::TypeID type, int amount);
    ~TaskSell() override;

    bool tick() override;
    std::string getName() override;
    float getCost() override;


};


#endif //ENGINE_2_TASKSELL_H