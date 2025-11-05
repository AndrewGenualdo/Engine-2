//
// Created by cobble on 11/4/2025.
//

#include "Task.h"

FarmingWorld *Task::world = nullptr;

Task::Task(LittleGuy *guy) {
    this->guy = guy;
}

void Task::setWorld(FarmingWorld *w) {
    world = w;
}

bool Task::update(float dt) {
    return true;
}

bool Task::isActive() {
    //???
    return false;
}

std::string Task::getName() {
    return "None";
}
