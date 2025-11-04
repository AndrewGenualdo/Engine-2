//
// Created by cobble on 8/20/2025.
//

#include "wall.h"
#include "../factory.h"

Wall::Wall(vec2 tile) {
    std::cout << "New " << name() << " Created at: " << tile.x << ", " << tile.y << std::endl;
    this->tile = tile;
}

Wall::~Wall() {
    Structure::cleanup();
}

void Wall::load() {
    Structure::load();
}

void Wall::cleanup() {
    Structure::cleanup();
}

void Wall::update(float dt) {
    Structure::update(dt);
}

void Wall::draw() {
    Structure::draw();
    Structure::draw();
    Texture2d::setColor(vec4(1));
    vec2 pos = FactoryScene::tilePos(tile);
    textures[name()].draw(pos.x + 13, pos.y + 24, FactoryScene::TILE_DIMS.x, FactoryScene::TILE_DIMS.y);
}

void Wall::setTexture(std::string texturePath) {
    Structure::setTexture(texturePath);
}

std::string Wall::name() const {
    return isVertical ? "Wall - Vertical" : "Wall - Horizontal";
}

void Wall::interract(Item *item, float dt) {
    if(isVertical) {
        item->vel.x = 0;
    } else {
        item->vel.y = 0;
    }
}
