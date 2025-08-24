//
// Created by cobble on 8/20/2025.
//

#include "track.h"
#include "../factory.h"

Animation Track::trackBasic = Animation();
Animation Track::trackEndLeft = Animation();
Animation Track::trackEndRight = Animation();
Animation Track::trackBasicToDiagonalRight = Animation();
Animation Track::trackDiagonalRight = Animation();

Track::Track(vec2 tile) {
    std::cout << "New " << name() << " Created at: " << tile.x << ", " << tile.y << std::endl;
    this->tile = tile;
    this->type = BASIC;
    this->speed = FactoryScene::TILE_DIMS.x / (trackBasic.delay * trackBasic.texture.getFrames() * 2.0f);
}

Track::~Track() {
    Structure::cleanup();
}

void Track::load() {
    Structure::load();
}

void Track::cleanup() {
    Structure::cleanup();
}

void Track::update(float dt) {
    Structure::update(dt);
}

void Track::draw() {
    Structure::draw();
    Texture2d::setColor(vec4(1));
    vec2 pos = FactoryScene::tilePos(tile);
    Animation* anim = nullptr;
    vec2 size = FactoryScene::TILE_DIMS;
    switch(type) {
        case BASIC: anim = &trackBasic; break;
        case END_LEFT: anim = &trackEndLeft; break;
        case END_RIGHT: anim = &trackEndRight; break;
        case BASIC_TO_DIAG_RIGHT: anim = &trackBasicToDiagonalRight; break;
        case DIAG_RIGHT: {
            anim = &trackDiagonalRight;
            pos -= 2.0f / 16.0f * size;
            size *= 21.0f / 16.0f;
            break;
        }
        default: anim = nullptr;
    }
    if(anim != nullptr) anim->drawLoop(FactoryScene::window->getTime() * (reversed ? -1 : 1), pos.x + 13, pos.y + 24, size.x, size.y);
    //if(anim != nullptr) anim->drawLoop(FactoryScene::window->getTime() * (reversed ? -1 : 1), pos.x + 13, pos.y + 24, size.x, size.y, FactoryScene::window->getTime());
    //textures[name()].draw(pos.x + 13, pos.y + 24, FactoryScene::TILE_DIMS.x, FactoryScene::TILE_DIMS.y);
}

void Track::setTexture(std::string texturePath) {
    Structure::setTexture(texturePath);
}

std::string Track::name() const {
    switch(type) {
        case BASIC: return "Track - Basic";
        case END_LEFT: return "Track - End Left";
        case END_RIGHT: return "Track - End Right";
        case BASIC_TO_DIAG_RIGHT: return "Track - Basic to Diagonal Right";
        case DIAG_RIGHT: return "Track - Diagonal Right";
        default: return "Track - Undefined";
    }
}

void Track::interract(Item *item, float dt) {
    item->vel = vec2(0);
    vec2 pos = FactoryScene::tilePos(tile);
    item->pos.y = pos.y + FactoryScene::TILE_DIMS.y * 0.5f;
    if(type == DIAG_RIGHT) item->pos.y += item->pos.x - pos.x - FactoryScene::TILE_DIMS.y * 0.4f;
    item->pos.x += speed * dt * (reversed ? -1 : 1);
}

void Track::updatePath() {
    paths.clear();
    if(type == BASIC) {
        //paths.push_back()
    }
}
