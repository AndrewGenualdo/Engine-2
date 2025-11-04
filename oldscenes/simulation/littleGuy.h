//
// Created by cobble on 8/12/2025.
//

#ifndef LITTLEGUY_H
#define LITTLEGUY_H
#include <glm/gtc/type_ptr.hpp>
#include "simulation.h"

using namespace glm;

class Team;

class LittleGuy {
public:
    int currentTile, nextTile;
    float x, y;
    float speed;
    float scale;
    float health, maxHealth;
    //Team* team;
    int teamIndex;
    int item;
    bool exists;

    LittleGuy();
    void action();
    bool interract(int tile);
    void takeDamage(float damage);

    void update(float deltaTime);
    void draw() const;

    static LittleGuy createGuy(int tile);
    vec2 getSpeed() const;
};



#endif //LITTLEGUY_H
