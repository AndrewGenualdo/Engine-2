//
// Created by cobble on 8/19/2025.
//

#include "bouncer.h"
#include "../factory.h"

Bouncer::Bouncer(vec2 tile) {
    std::cout << "New " << name() << " Created at: " << tile.x << ", " << tile.y << std::endl;
    this->tile = tile;
}

Bouncer::~Bouncer() {
    Structure::cleanup();
}

void Bouncer::load() {
    Structure::load();
}

void Bouncer::cleanup() {
    Structure::cleanup();
}

void Bouncer::update(float dt) {
    Structure::update(dt);
}

void Bouncer::draw() {
    Structure::draw();
    Texture2d::setColor(vec4(1));
    vec2 pos = FactoryScene::tilePos(tile);
    textures[name()].draw(pos.x + 13, pos.y + 24, FactoryScene::TILE_DIMS.x, FactoryScene::TILE_DIMS.y);
}

void Bouncer::setTexture(std::string texturePath) {
    Structure::setTexture(texturePath);
}

std::string Bouncer::name() const {
    return isLeft ? "Bouncer - Left" : "Bouncer - Right";
}

void Bouncer::interract(Item *item, float dt) {
    item->vel.y = abs(item->vel.y) + abs(item->vel.x) * 0.5f;
    item->vel.x += FactoryScene::GRAVITY * (isLeft ? -1 : 1);
}
