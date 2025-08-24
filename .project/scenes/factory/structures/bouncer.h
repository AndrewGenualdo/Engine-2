//
// Created by cobble on 8/19/2025.
//

#ifndef BOUNCER_H
#define BOUNCER_H
#include "structure.h"


class Bouncer : public Structure {
public:
    explicit Bouncer() = default;
    explicit Bouncer(vec2 tile);
    ~Bouncer() override;

    void load() override;
    void cleanup() override;
    void update(float dt) override;
    void draw() override;
    void setTexture(std::string texturePath) override;
    std::string name() const override;
    void interract(Item *item, float dt) override;

    bool isLeft = true;
};



#endif //BOUNCER_H
