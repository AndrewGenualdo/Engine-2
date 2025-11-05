//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_TASKRETRIEVEITEM_H
#define ENGINE_2_TASKRETRIEVEITEM_H
#include "Task.h"


class Item;

class TaskRetrieveItem : public Task {


public:
    TaskRetrieveItem(LittleGuy *guy, Item item, int amount);
    bool update(float dt) override;
    std::string getName() override;
};


#endif //ENGINE_2_TASKRETRIEVEITEM_H