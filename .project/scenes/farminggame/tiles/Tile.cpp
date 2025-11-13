//
// Created by cobble on 11/5/2025.
//

#include "Tile.h"
#include "../FarmingWorld.h"

Tile::Tile(const ivec2 tile) : FarmingObject(tile) {
    tileExists = false;
}

void Tile::update(float dt) {

}

void Tile::tick() {

}

void Tile::draw(bool bind) {

}

FarmingObject::TypeID Tile::getType() const {
    return TypeID::TILE;
}

bool Tile::destroy() {
    if (world != nullptr) {
        if (world->getTile(tile) != nullptr) {
            delete world->getTile(tile);
            world->setTile(tile, new Tile(tile));
            return true;
        }
    }
    return false;
}

std::string Tile::getConfig() {
    return FarmingObject::getConfig()+"-\n";
}

void Tile::loadConfig(const std::string &line, int i) {
    if (i == 1) {
        //load stuff for Tile.h here
    } else FarmingObject::loadConfig(line, i);
}

bool Tile::exists() const {
    return tileExists;
}
