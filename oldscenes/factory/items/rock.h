//
// Created by cobble on 8/19/2025.
//

#ifndef ROCK_H
#define ROCK_H

#include "item.h"

class Rock : public Item {
public:
    explicit Rock();
    ~Rock() override;

    void load() override;
    void cleanup() override;
    void update(float dt) override;
    void draw() override;
    void setTexture(std::string texturePath) override;
    std::string name() const override;
};



#endif //ROCK_H
