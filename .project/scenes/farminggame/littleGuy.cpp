//
// Created by drewg on 11/4/2025.
//

#include "littleGuy.h"

FarmingWorld *LittleGuy::world = nullptr;

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

void LittleGuy::setWorld(FarmingWorld *w) {
    LittleGuy::world = w;
}

void LittleGuy::update(float dt) {

}

void LittleGuy::draw() {

}
