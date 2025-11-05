//
// Created by drewg on 11/4/2025.
//

#include "littleGuy.h"

#include "tasks/Task.h"

FarmingWorld *LittleGuy::world = nullptr;
MultiTexture2d LittleGuy::texture = MultiTexture2d();
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

void LittleGuy::setTexture(const std::string &path, int width, int height, int frames) {
    LittleGuy::width = width;
    LittleGuy::height = height;
    texture = MultiTexture2d(path, frames);
}

void LittleGuy::update(float dt) {
    if (task == nullptr) return;
    if (task->update(dt)) {
        delete task;
        task = nullptr;
    }
    for (int i = 0; i < world->objects.size(); i++) {
        if (world->objects[i] != nullptr) {
            Item *item = dynamic_cast<Item *>(world->objects[i]);
            if (item && length(item->pos - pos) < FarmingWorld::TILE_WIDTH) {
                giveItem(item);
                world->objects.erase(world->objects.begin() + i);
                break;
            }
        }
    }
}

void LittleGuy::tick() {
    if (task != nullptr) {
        task->tick();
    }
}

void LittleGuy::draw(bool bind) const {
    texture.draw(pos.x, pos.y, width, height, items.empty() ? 0 : 1, bind);
    for (int i = 0; i < items.size();i++) {
        items[i]->draw(-items[i]->pos.x + pos.x, -items[i]->pos.y + pos.y + (items.size() - i) * FarmingWorld::TILE_HEIGHT, i == 0);
    }
}

void LittleGuy::clearObjects() {
    for (int i = 0; i < items.size(); i++) {
        if (items[i] != nullptr) {
            delete items[i];
            items[i] = nullptr;
        }
    }
    items.clear();
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

void LittleGuy::giveItem(Item *item) {
    items.push_back(item);
}

void LittleGuy::setTask(Task *task) {
    if (this->task != nullptr) {
        delete this->task;
        this->task = nullptr;
    }
    this->task = task;
}
