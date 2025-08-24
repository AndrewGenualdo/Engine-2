//
// Created by cobble on 8/20/2025.
//

#ifndef WALL_H
#define WALL_H
#include "structure.h"


class Wall : public Structure {
public:
    explicit Wall() = default;
    explicit Wall(vec2 tile);
    ~Wall() override;

    void load() override;
    void cleanup() override;
    void update(float dt) override;
    void draw() override;
    void setTexture(std::string texturePath) override;
    std::string name() const override;
    void interract(Item *item, float dt) override;

    bool isVertical = true;
};



#endif //WALL_H
