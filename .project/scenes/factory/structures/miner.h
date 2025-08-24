//
// Created by cobble on 8/19/2025.
//

#pragma once

#ifndef MINER_H
#define MINER_H

#include "structure.h"

class Miner : public Structure {
public:
    explicit Miner() = default;
    explicit Miner(vec2 tile);
    ~Miner() override;

    void load() override;
    void cleanup() override;
    void update(float dt) override;
    void draw() override;
    void setTexture(std::string texturePath) override;
    std::string name() const override;
    void interract(Item *item, float dt) override;

    float timeBetween = 1.0f;
    float timeUntil = 1.0f;
};



#endif //MINER_H
