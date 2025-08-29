//
// Created by cobble on 8/29/2025.
//

#ifndef CART_H
#define CART_H
#include <glm/vec3.hpp>

#include "block.h"

using namespace glm;

class Cart {
public:
        Block* block = nullptr;
        vec3 blockPos = vec3(0);
        bool backwards = false;
        float speed = 0.0f;
        float progress = 0.0f; //0.0 -> 1.0 based on how far along the current track it is

        void setSpeed(float speed) {
            this->speed = speed;
        }

        void setBlock(vec3 blockPos);

        explicit Cart(vec3 blockPos = vec3(-1), bool backwards = false, float speed = 0.0f, float progress = 0.0f) {
            this->setBlock(blockPos);
            this->backwards = backwards;
            setSpeed(speed);
            this->progress = progress;
        }

        void update(float dt);

        void draw(float w, float h) const;
    };



#endif //CART_H
