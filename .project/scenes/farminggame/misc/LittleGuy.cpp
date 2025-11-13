//
// Created by drewg on 11/4/2025.
//

#include "LittleGuy.h"

#include "../tasks/Task.h"

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
    if (task != nullptr) {
        if (task->update(dt)) {
            task->setActive(false);
            delete task;
            task = nullptr;
        }
    }
}

void LittleGuy::tick() {
    if (task != nullptr) {
        if (task->tick()) {
            task->setActive(false);
            delete task;
            task = nullptr;
        }
    }
}

void LittleGuy::draw(const bool bind) {
    int frame = wasLastDirLeft ? 0 : 1;
    frame += itemList.empty() ? 0 : 2;
    texture.draw(pos.x - width * 0.5f, pos.y - height * 0.5f, width, height, frame, bind);
    for (int i = itemList.size() - 1; i >= 0; i--) {
        Item::draw(pos.x, pos.y + (0.722f * 0.772f * height) + (itemList.size() - i - 1) * FarmingWorld::TILE_HEIGHT * 0.5f, itemList[i], i == itemList.size() - 1);
    }
}

std::string LittleGuy::getConfig() {
    std::string output = FarmingObject::getConfig();
    output += std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(speed) + "\n";
    for (auto & item : itemList) {
        output += "=+"+getData<ObjectData>(item)->configKey+"\n";
    }
    return output;
}

void LittleGuy::loadConfig(const std::string &line, const int i) {
    if (i > 1) {
        for (auto const& [type, data] : objectData) {
            if (dynamic_cast<Item::ItemData*>(data)) { //check if item
                if ("=+"+data->configKey == line) {
                    itemList.push_back(type);
                    break;
                }
            }
        }
    }
    else if (i == 1) {
        std::istringstream iss(line);
        iss >> pos.x >> pos.y >> speed;
    } else FarmingObject::loadConfig(line, i);
}

void LittleGuy::clearObjects() {
    /*for (auto & item : items) {
        if (item != nullptr) {
            delete item;
            item = nullptr;
        }
    }
    items.clear();*/
    itemList.clear();
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

int LittleGuy::hasItem(TypeID type) {
    int amount = 0;
    if (getData<Item::ItemData>(type)) {
        for (const auto & item : itemList) {
            if (item == type) {
                amount++;
            }
        }
    }
    return amount;
}

Item* LittleGuy::takeItem(const TypeID type) {
    if (getData<Item::ItemData>(type)) {
        for (int i = 0; i < itemList.size(); i++) {
            if (itemList[i] == type) {
                itemList.erase(itemList.begin() + i);
                Item *item = FarmingWorld::createItem(type);
                if (item == nullptr) return nullptr;
                item->pos = pos;
                item->tile = tile;
                return item;
            }
        }
    }
    return nullptr;
}

void LittleGuy::giveItem(const TypeID type) {
    itemList.push_back(type);
}

Task * LittleGuy::getTask() const {
    return task;
}

void LittleGuy::setTask(Task *task) {
    if (this->task != nullptr) {
        this->task->setActive(false);
        delete this->task;
        this->task = nullptr;
    }
    this->task = task;
}
