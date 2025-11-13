//
// Created by drewg on 11/13/2025.
//

#ifndef ENGINE_2_LITTLEGUYMANAGER_H
#define ENGINE_2_LITTLEGUYMANAGER_H
#include "LittleGuy.h"


class LittleGuyManager {

    static FarmingWorld *world;

    std::vector<LittleGuy*> guys;
    std::vector<std::vector<Task*>> tasks; //each of the outer layer is a list of tasks for a single guy to complete. The outer tasks can be given out as soon as they are available.
    FarmingObject::TypeID goalType = FarmingObject::TypeID::NONE;
    int goalAmount = 0;

public:
    static void setWorld(FarmingWorld *world);

    LittleGuyManager();
    ~LittleGuyManager();

    void update(float dt);
    void draw(bool bind) const;
    void cleanup();

    int setGoal(FarmingObject::TypeID goal, int amount);

    bool addGuy(LittleGuy* guy);
    [[nodiscard]] int getCount() const;

private:
    int createTasks(FarmingObject::TypeID type, int amount);
};


#endif //ENGINE_2_LITTLEGUYMANAGER_H