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

Item::Item(const ivec2 tile) : FarmingObject(tile) {
    this->pos = FarmingWorld::getTilePos(tile.x, tile.y);
}

Item::Item(const vec2 pos) : FarmingObject(FarmingWorld::getTileFromPos(pos)){
    this->pos = pos;
}

FarmingObject::TypeID Item::getType() const {
    return TypeID::ITEM;
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
    itemsTexture.draw(pos.x + offsetX - (width * 0.5f), pos.y + offsetY - (height * 0.5f), width, height, getData<ItemData>(getType())->textureIndex, bind);
}

std::string Item::getConfigKey() {
    return FarmingObject::getConfigKey() + "ITEM";
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
