//
// Created by cobble on 11/5/2025.
//

#include "Item.h"
#include "../farmingWorld.h"

MultiTexture2d Item::itemsTexture = MultiTexture2d();
int Item::width = 1;
int Item::height = 1;

void Item::setTexture(const std::string &path, int width, int height, int items) {
    itemsTexture = MultiTexture2d(path, items);
    Item::width = width;
    Item::height = height;
}

Item::Item(ItemType type, ivec2 tile) {
    this->type = type;
    this->pos = FarmingWorld::getTilePos(tile.x, tile.y);
    this->tile = tile;
}

Item::Item(ItemType type, vec2 pos) {
    this->type = type;
    this->tile = FarmingWorld::getTileFromPos(pos);
    this->pos = pos;
}

void Item::update(float dt) {

}

void Item::draw(bool bind) {
    itemsTexture.draw(pos.x, pos.y, width, height, type, bind);
}
