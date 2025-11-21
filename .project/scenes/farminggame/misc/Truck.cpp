//
// Created by cobble on 11/20/2025.
//

#include "Truck.h"
#include "../FarmingWorld.h"
MultiTexture2d Truck::texture = MultiTexture2d();

Truck::Truck() {
}

bool Truck::available() const {
    return state == HERE;
}

void Truck::update(float dt) {
    if (timeLeftInState == -1) return;
    switch (state) {
        case ENTERING: pos = vec2(pos.x - dt * FarmingWorld::TILE_WIDTH * 2, pos.y); break;
        case EXITING: pos = vec2(pos.x + dt * FarmingWorld::TILE_WIDTH * 2, pos.y); break;
        default: pos = vec2(0,0); break;
    }
    timeLeftInState -= dt;
    if (timeLeftInState <= 0.0f) {
        timeLeftInState = 0.0f;
        switch (state) {
            case ENTERING: changeState(OPENING); break;
            case OPENING: changeState(HERE); break;
            case HERE: changeState(CLOSING); break;
            case CLOSING: changeState(EXITING); break;
            case EXITING: changeState(GONE); break;
            default: break;
        }
    }
}

void Truck::draw() const {
    if (state != GONE) {
        vec2 p = FarmingWorld::getTilePos(FarmingWorld::TILES_HORIZ - 2, 0) + pos;

        int frame = 0;
        switch (state) {
            case ENTERING:
            case EXITING: frame = static_cast<int>(timeLeftInState * 8) % 2 == 0 ? 3 : 4; break;
            case HERE: frame = 0; break;
            case OPENING: frame = static_cast<int>(timeLeftInState * 3); break;
            case CLOSING: frame = 3 - static_cast<int>(timeLeftInState * 3); break;
            default: break;
        }

        texture.draw(p.x, p.y, FarmingWorld::TILE_WIDTH * 3, FarmingWorld::TILE_HEIGHT * 3, frame);
    }
}

void Truck::enter() {
    changeState(ENTERING);
}

void Truck::changeState(TruckState newState) {
    if (state == newState) return;
    if (newState == ENTERING) {
        pos = vec2(FarmingWorld::TILE_WIDTH * 2, 0);
        timeLeftInState = 1.0f;
    } else if (newState == HERE || newState == EXITING || newState == OPENING || newState == CLOSING) {
        pos = vec2(0, 0);
        timeLeftInState = 1.0f;
    } else {
        pos = vec2(0, 0);
        timeLeftInState = -1.0f;
    }
    state = newState;
}