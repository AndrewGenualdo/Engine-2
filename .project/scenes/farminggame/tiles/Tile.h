//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_TILE_H
#define ENGINE_2_TILE_H

#include <cobb/misc/texture2d.h>
#include <glm/vec2.hpp>

using namespace glm;
using namespace cobb;

class Tile {

    ivec2 tile;

public:
    Tile() = default;
    Tile(ivec2 tile);

};


#endif //ENGINE_2_TILE_H