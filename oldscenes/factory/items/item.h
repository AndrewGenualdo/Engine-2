//
// Created by cobble on 8/19/2025.
//

#ifndef ITEM_H
#define ITEM_H

#include <map>
#include <glm/vec2.hpp>

#include "cobb/misc/texture2d.h"

using namespace cobb;
using namespace glm;

class Item {
public:
    vec2 pos, vel;
    float mass, scale; //gravity multiplier

    static std::map<std::string, Texture2d> textures;

    explicit Item();
    virtual ~Item();

    virtual void load();
    virtual void cleanup();
    virtual void update(float dt);
    virtual void draw();
    virtual void setTexture(std::string texturePath);
    virtual std::string name() const;
};



#endif //ITEM_H
