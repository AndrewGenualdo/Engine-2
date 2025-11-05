//
// Created by drewg on 11/4/2025.
//

#ifndef ENGINE_2_LITTLEGUY_H
#define ENGINE_2_LITTLEGUY_H


#include "glm/vec2.hpp"
#include "farmingWorld.h"
#include "items/Item.h"
#include "tasks/Task.h"

using namespace glm;

class LittleGuy {

    static FarmingWorld *world;
    static MultiTexture2d texture;
    static int width, height;

    Task *task = nullptr;
    std::vector<Item*> items;

    vec2 pos;
    ivec2 tile;
    float speed = 150;

public:
    static void setWorld(FarmingWorld *w);
    static void setTexture(const std::string &path, int width, int height, int frames);

    LittleGuy();
    explicit LittleGuy(ivec2 tile);
    explicit LittleGuy(vec2 pos);
    ~LittleGuy();

    void update(float dt);
    void tick();
    void draw(bool bind = true) const;

    void clearObjects();

    [[nodiscard]] vec2 getPos() const;
    void move(vec2 diff);
    void setPos(vec2 pos);

    [[nodiscard]] ivec2 getTile() const;
    void setTile(ivec2 tile);

    [[nodiscard]] float getSpeed() const;
    void setSpeed(float speed);

    void giveItem(Item *item);

    void setTask(Task *task);
};


#endif //ENGINE_2_LITTLEGUY_H
