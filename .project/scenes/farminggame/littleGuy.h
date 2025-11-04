//
// Created by drewg on 11/4/2025.
//

#ifndef ENGINE_2_LITTLEGUY_H
#define ENGINE_2_LITTLEGUY_H


#include "glm/vec2.hpp"
#include "farmingWorld.h"

using namespace glm;



class LittleGuy {

    static FarmingWorld *world;

    enum TaskType {
        NONE,
        WALK
    };

    TaskType task = NONE;
    vec2 pos;
    ivec2 tile;

public:
    static void setWorld(FarmingWorld *w);

    LittleGuy();
    explicit LittleGuy(ivec2 tile);
    explicit LittleGuy(vec2 pos);

    void update(float dt);
    void draw();
};


#endif //ENGINE_2_LITTLEGUY_H
