//
// Created by cobble on 8/19/2025.
//

#include "item.h"
#include "../factory.h"

std::map<std::string, Texture2d> Item::textures = std::map<std::string, Texture2d>();

Item::Item() {
    pos = vec2(0);
    vel = vec2(0);
    mass = 1.0f;
    scale = 1.0f;
}

Item::~Item() {
    cleanup();
}

void Item::load() {
}

void Item::cleanup() {
}

void Item::update(float dt) {
    vel.y -= mass * FactoryScene::GRAVITY * dt;
    pos += vel * dt;
    if(pos.x < 0 || pos.y < 0 || pos.x > Window::GAME_WIDTH || pos.y > Window::GAME_HEIGHT) FactoryScene::itemsToDestroy.push_back(this);
}

void Item::draw() {
}

void Item::setTexture(std::string texturePath) {
    textures[name()] = Texture2d(texturePath);
}

std::string Item::name() const {
    return "Undefined";
}
