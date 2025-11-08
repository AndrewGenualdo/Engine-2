//
// Created by cobble on 11/8/2025.
//

#include "TaskWait.h"

TaskWait::TaskWait(LittleGuy *guy, int ticks) {
    this->guy = guy;
    this->ticks = ticks;
}

TaskWait::~TaskWait() = default;

bool TaskWait::tick() {
    if (ticks > 0) {
        ticks--;
        return false;
    }
    return true;
}

std::string TaskWait::getName() {
    return "Wait: {" + std::to_string(ticks) + "} ";
}

float TaskWait::getCost() {
    return ticks;
}

void TaskWait::clear() {

}
