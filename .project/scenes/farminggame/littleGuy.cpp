//
// Created by drewg on 11/4/2025.
//

#include "littleGuy.h"

#include "tasks/Task.h"

FarmingWorld *LittleGuy::world = nullptr;
Texture2d LittleGuy::texture = Texture2d();
int LittleGuy::width = 1;
int LittleGuy::height = 1;

LittleGuy::LittleGuy(ivec2 tile) {
    this->pos = FarmingWorld::getTilePos(tile.x, tile.y);
    this->tile = tile;
}

LittleGuy::LittleGuy() {
    tile = ivec2(0);
    pos = FarmingWorld::getTilePos(0, 0);
}

LittleGuy::LittleGuy(vec2 pos) {
    this->tile = FarmingWorld::getTileFromPos(pos);
    this->pos = pos;
}

LittleGuy::~LittleGuy() {
    if (task != nullptr) {
        delete task;
        task = nullptr;
    }
}

void LittleGuy::setWorld(FarmingWorld *w) {
    Task::setWorld(w);
    world = w;
}

void LittleGuy::setTexture(const std::string &path, int width, int height) {
    LittleGuy::width = width;
    LittleGuy::height = height;
    texture = Texture2d(path, GL_NEAREST, GL_TEXTURE_WRAP_S);;
}

void LittleGuy::update(float dt) {
    if (task == nullptr) return;
    if (task->update(dt)) {
        delete task;
        task = nullptr;
    }
}

void LittleGuy::draw(bool bind) const {
    texture.draw(pos.x, pos.y, width, height, bind);
}

vec2 LittleGuy::getPos() const {
    return pos;
}

void LittleGuy::move(vec2 diff) {
    pos += diff;
}

void LittleGuy::setPos(vec2 pos) {
    this->pos = pos;
}

ivec2 LittleGuy::getTile() const {
    return tile;
}

void LittleGuy::setTile(ivec2 tile) {
    this->tile = tile;
}

float LittleGuy::getSpeed() const {
    return speed;
}

void LittleGuy::setSpeed(float speed) {
    this->speed = speed;
}

void LittleGuy::setTask(Task *task) {
    if (this->task != nullptr) {
        delete this->task;
        this->task = nullptr;
    }
    this->task = task;
}
