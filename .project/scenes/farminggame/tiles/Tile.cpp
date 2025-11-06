//
// Created by cobble on 11/5/2025.
//

#include "Tile.h"

Tile::Tile(const ObjectType type, const ivec2 tile) : FarmingObject(type, tile) {

}

void Tile::update(float dt) {

}

void Tile::tick() {

}

void Tile::draw(bool bind) {

}

std::string Tile::getConfigKey() {
    return FarmingObject::getConfigKey() + "TILE";
}

std::string Tile::getConfig() {
    return FarmingObject::getConfig()+"-\n";
}

void Tile::loadConfig(const std::string &line, int i) {
    if (i == 1) {
        //load stuff for Tile.h here
    } else FarmingObject::loadConfig(line, i);
}
