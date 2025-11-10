//
// Created by cobble on 11/5/2025.
//

#include "Item.h"



#include "../farmingWorld.h"

MultiTexture2d Item::itemsTexture = MultiTexture2d();
int Item::width = 1;
int Item::height = 1;

void Item::setTexture(const std::string &path, const int width, const int height, const int items) {
    itemsTexture = MultiTexture2d(path, items);
    Item::width = width;
    Item::height = height;
}

void Item::draw(float x, float y, float width, float height, int frame, bool bind) {
    itemsTexture.draw(x, y, width, height, frame, bind);
}

Item::Item(const ivec2 tile) : FarmingObject(tile) {
    this->pos = FarmingWorld::getTilePos(tile.x, tile.y);
}

Item::Item(const vec2 pos) : FarmingObject(FarmingWorld::getTileFromPos(pos)){
    this->pos = pos;
}

FarmingObject::TypeID Item::getType() const {
    return TypeID::ITEM;
}

bool Item::destroy() {
    if (world != nullptr) {
        for (int i = 0; i < world->items.size(); i++) {
            if (world->items[i] == this) {
                delete world->items[i];
                world->items.erase(world->items.begin() + i);
                return true;
            }
        }
    }
    return false;
}

void Item::update(float dt) {

}

void Item::tick() {
    FarmingObject::tick();
}

void Item::draw(bool bind) {
    draw(0, 0, bind);
}

void Item::draw(float offsetX, float offsetY, bool bind) {
    draw(pos.x + offsetX, pos.y + offsetY, getType(), bind);
}

void Item::draw(float x, float y, TypeID type, bool bind) {
    itemsTexture.draw(x - (width * 0.5f), y - (height * 0.5f), width, height, getData<ItemData>(type)->textureIndex, bind);
}

std::string Item::getConfig() {
    return FarmingObject::getConfig() + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + "\n";
}

void Item::loadConfig(const std::string &line, int i) {
    if (i == 1) {
        std::istringstream iss(line);
        iss >> pos.x >> pos.y;
    } else {
        FarmingObject::loadConfig(line, i);
    }
}
