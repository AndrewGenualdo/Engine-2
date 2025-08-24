//
// Created by cobble on 8/19/2025.
//

#include "structure.h"

std::map<std::string, Texture2d> Structure::textures = std::map<std::string, Texture2d>();

Structure::Structure(vec2 tile) {
    this->tile = tile;
}

Structure::~Structure() {
    cleanup();
}

void Structure::setTexture(std::string texturePath) {
    textures[name()] = Texture2d(texturePath);
}

void Structure::load() {

}

void Structure::cleanup() {
}

void Structure::update(float dt) {
}

void Structure::draw() {

}

std::string Structure::name() const {
    return "Undefined";
}

void Structure::interract(Item *item, float dt) {

}
