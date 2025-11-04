//
// Created by cobble on 8/19/2025.
//

#include "collector.h"
#include "../factory.h"


Collector::Collector(vec2 tile) {
    std::cout << "New " << name() << " Created at: " << tile.x << ", " << tile.y << std::endl;
    this->tile = tile;
}

Collector::~Collector() {
    Structure::cleanup();
}

void Collector::load() {
    Structure::load();
}

void Collector::cleanup() {
    Structure::cleanup();
}

void Collector::update(float dt) {
    Structure::update(dt);
}

void Collector::draw() {
    Structure::draw();
    Texture2d::setColor(vec4(1));
    vec2 pos = FactoryScene::tilePos(tile);
    textures[name()].draw(pos.x + 13, pos.y + 24, FactoryScene::TILE_DIMS.x, FactoryScene::TILE_DIMS.y);
}

void Collector::setTexture(std::string texturePath) {
    Structure::setTexture(texturePath);
}

std::string Collector::name() const {
    return "Collector";
}

void Collector::interract(Item *item, float dt) {
    FactoryScene::score++;
    FactoryScene::itemsToDestroy.push_back(item);
}
