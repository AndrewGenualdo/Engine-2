//
// Created by cobble on 8/19/2025.
//

#include "rock.h"
#include "../factory.h"

Rock::Rock() {
    std::cout << "New " << name() << " Created at: " << pos.x << ", " << pos.y << std::endl;
    this->scale = 0.5f;
}

Rock::~Rock() {
}

void Rock::load() {
    Item::load();
}

void Rock::cleanup() {
    Item::cleanup();
}

void Rock::update(float dt) {
    Item::update(dt);
    Structure *tile = FactoryScene::getTile(FactoryScene::getTileAtPos(pos));
    if(tile != nullptr) tile->interract(this, dt);
}

void Rock::draw() {
    Item::draw();
    Texture2d::setColor(vec4(1));
    textures[name()].draw(pos.x + 13 - FactoryScene::TILE_DIMS.x * 0.5f * scale, pos.y + 24 - FactoryScene::TILE_DIMS.y * 0.5f * scale, FactoryScene::TILE_DIMS.x * scale, FactoryScene::TILE_DIMS.y * scale);
}

void Rock::setTexture(std::string texturePath) {
    Item::setTexture(texturePath);
}

std::string Rock::name() const {
    return Item::name();
}
