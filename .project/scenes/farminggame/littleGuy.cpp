//
// Created by drewg on 11/4/2025.
//

#include "littleGuy.h"

#include "tasks/Task.h"

FarmingWorld *LittleGuy::world = nullptr;
MultiTexture2d LittleGuy::texture = MultiTexture2d();
int LittleGuy::width = 1;
int LittleGuy::height = 1;

LittleGuy::LittleGuy(const ivec2 tile) {
    this->pos = FarmingWorld::getTilePos(tile.x, tile.y);
    this->tile = tile;
}

LittleGuy::LittleGuy() {
    tile = ivec2(0);
    pos = FarmingWorld::getTilePos(0, 0);
}

LittleGuy::LittleGuy(const vec2 pos) {
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

void LittleGuy::setTexture(const std::string &path,const  int width, const int height, const int frames) {
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

void LittleGuy::draw(const bool bind) {
    int frame = wasLastDirLeft ? 0 : 1;
    frame += items.empty() ? 0 : 2;
    texture.draw(pos.x - width * 0.5f, pos.y - height * 0.5f, width, height, frame, bind);
    for (int i = items.size() - 1; i >= 0; i--) {
        items[i]->draw(-items[i]->pos.x + pos.x, -items[i]->pos.y + pos.y + (0.722f * 0.772f * height) + (items.size() - i - 1) * FarmingWorld::TILE_HEIGHT * 0.5f, i == items.size() - 1);
    }
}

std::string LittleGuy::getConfigKey() {
    return "LITTLE_GUY";
}

std::string LittleGuy::getConfig() {
    std::string output = FarmingObject::getConfig();
    output += std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(speed) + "\n";
    for (auto & item : items) {
        output += "="+item->getConfigKey() + "\n";
        output += item->getConfig();
    }
    return output;
}

void LittleGuy::loadConfig(const std::string &line, const int i) {
    if (i == 1) {
        std::istringstream iss(line);
        iss >> pos.x >> pos.y >> speed;
    } else FarmingObject::loadConfig(line, i);
}

void LittleGuy::clearObjects() {
    for (auto & item : items) {
        if (item != nullptr) {
            delete item;
            item = nullptr;
        }
    }
    items.clear();
}

vec2 LittleGuy::getPos() const {
    return pos;
}

void LittleGuy::move(vec2 diff) {
    if (diff.x < 0) wasLastDirLeft = true;
    else if (diff.x > 0) wasLastDirLeft = false;
    pos += diff;
}

void LittleGuy::setPos(const vec2 pos) {
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

void LittleGuy::setSpeed(const float speed) {
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
