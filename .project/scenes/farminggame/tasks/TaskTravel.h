//
// Created by cobble on 11/4/2025.
//

#ifndef ENGINE_2_TASKTRAVEL_H
#define ENGINE_2_TASKTRAVEL_H
#include <vector>

#include "Task.h"


class TaskTravel : public Task {

    bool generatePath();
public:
    ivec2 goal;
    std::vector<ivec2> path;
    int pathIndex = 0;

    TaskTravel(LittleGuy *guy, ivec2 goal);
    bool update(float dt) override;
    std::string getName() override;
};


#endif //ENGINE_2_TASKTRAVEL_H