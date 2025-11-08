//
// Created by cobble on 11/8/2025.
//

#ifndef ENGINE_2_TASKWAIT_H
#define ENGINE_2_TASKWAIT_H
#include "Task.h"


class TaskWait : public Task {
    int ticks;
public:
    TaskWait(LittleGuy *guy, int ticks);
    ~TaskWait() override;

    bool tick() override;
    std::string getName() override;
    float getCost() override;
    void clear() override;

};


#endif //ENGINE_2_TASKWAIT_H