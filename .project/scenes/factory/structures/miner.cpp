//
// Created by cobble on 8/19/2025.
//

#include "miner.h"
#include "../factory.h"

Miner::Miner(vec2 tile) {
    std::cout << "New " << name() << " Created at: " << tile.x << ", " << tile.y << std::endl;
    this->tile = tile;
}

Miner::~Miner() {
    Structure::cleanup();
}

void Miner::load() {
    Structure::load();
}

void Miner::cleanup() {
    Structure::cleanup();
}

void Miner::update(float dt) {
    Structure::update(dt);
    timeUntil -= dt;
    if(timeUntil < 0) {
        Rock* rock = new Rock();
        FactoryScene::items.push_back(rock);
        rock->pos = FactoryScene::tilePos(tile) + FactoryScene::TILE_DIMS * 0.5f * rock->scale;
        rock->vel = vec2(0);
        timeUntil = timeBetween;
    }
}

void Miner::draw() {
    Structure::draw();
    Texture2d::setColor(vec4(1));
    vec2 pos = FactoryScene::tilePos(tile);
    textures[name()].draw(pos.x + 13, pos.y + 24, FactoryScene::TILE_DIMS.x, FactoryScene::TILE_DIMS.y);
}

void Miner::setTexture(std::string texturePath) {
    Structure::setTexture(texturePath);
}

std::string Miner::name() const {
    return "Miner";
}

void Miner::interract(Item *item, float dt) {

}
