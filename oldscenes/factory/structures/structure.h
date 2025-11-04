//
// Created by cobble on 8/19/2025.
//

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <map>
#include <glm/vec2.hpp>
#include "cobb/misc/texture2d.h"
#include "../items/item.h"

using namespace cobb;
using namespace glm;

class Structure {

public:
    vec2 tile = vec2(0);
    static std::map<std::string, Texture2d> textures;

    explicit Structure() = default;
    explicit Structure(vec2 tile);
    virtual ~Structure();



    virtual void load();
    virtual void cleanup();
    virtual void update(float dt);
    virtual void draw();
    virtual void setTexture(std::string texturePath);
    virtual std::string name() const;
    virtual void interract(Item *item, float dt);

};



#endif //STRUCTURE_H
