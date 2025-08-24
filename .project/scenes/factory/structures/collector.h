//
// Created by cobble on 8/19/2025.
//

#pragma once

#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "structure.h"

class Collector : public Structure {
public:
    explicit Collector() = default;
    explicit Collector(vec2 tile);
    ~Collector() override;

    void load() override;
    void cleanup() override;
    void update(float dt) override;
    void draw() override;
    void setTexture(std::string texturePath) override;
    std::string name() const override;
    void interract(Item *item, float dt) override;
};



#endif //COLLECTOR_H
