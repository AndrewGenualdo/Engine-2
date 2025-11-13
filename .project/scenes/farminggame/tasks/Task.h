//
// Created by cobble on 11/4/2025.
//

#ifndef ENGINE_2_TASK_H
#define ENGINE_2_TASK_H

#include <glm/vec2.hpp>
#include "../FarmingWorld.h"

class LittleGuy;
using namespace glm;

class Task {
protected:
    static FarmingWorld *world;
    LittleGuy *guy = nullptr;

public:
    Task() = default;
    Task(LittleGuy *guy);
    virtual ~Task() = default;

    static void setWorld(FarmingWorld *w);

    virtual bool update(float dt);
    virtual bool tick();
    virtual float getCost();
    virtual void setActive(bool active);
    virtual bool isActive();

    virtual std::string getName();
};


#endif //ENGINE_2_TASK_H