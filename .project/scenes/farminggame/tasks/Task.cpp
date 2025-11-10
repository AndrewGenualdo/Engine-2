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
    std::cout << "you probably called the wrong task update/tick function! [update]" << std::endl;
    return true;
}

bool Task::tick() {
    std::cout << "you probably called the wrong task update/tick function! [tick]" << std::endl;
    return true;
}

float Task::getCost() {
    return 0.0f;
}

void Task::setActive(bool active) {

}

bool Task::isActive() {
    //???
    return false;
}

std::string Task::getName() {
    return "None";
}
