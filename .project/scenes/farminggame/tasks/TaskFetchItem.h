//
// Created by cobble on 11/7/2025.
//

#ifndef ENGINE_2_TASKFETCHITEM_H
#define ENGINE_2_TASKFETCHITEM_H
#include "Task.h"
#include "TaskRetrieveItem.h"


class TaskFetchItem : public Task {

    ivec2 returnToTile = ivec2(0);

    TaskRetrieveItem *taskRetrieveItem = nullptr;
    TaskTravel *taskTravel = nullptr;

public:
    TaskFetchItem(LittleGuy *guy, FarmingObject::TypeID type, int amount, ivec2 returnToTile);
    ~TaskFetchItem() override;

    bool update(float dt) override;
    bool tick() override;
    std::string getName() override;
    float getCost() override;
};


#endif //ENGINE_2_TASKFETCHITEM_H