//
// Created by cobble on 11/4/2025.
//

#ifndef ENGINE_2_TASKTRAVEL_H
#define ENGINE_2_TASKTRAVEL_H
#include <vector>

#include "Task.h"


class TaskTravel : public Task {


public:
    ivec2 start, goal;
    std::vector<ivec2> path;
    float cost = 0;
    int pathIndex = 0;

    TaskTravel() = default;
    TaskTravel(LittleGuy *guy, ivec2 goal);
    TaskTravel(LittleGuy *guy, ivec2 start, ivec2 goal);
    bool update(float dt) override;
    float getCost() override;
    void setGuy(LittleGuy *guy) override;
    bool generatePath();

    std::string getName() override;
};


#endif //ENGINE_2_TASKTRAVEL_H