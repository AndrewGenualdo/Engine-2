//
// Created by cobble on 8/29/2025.
//

#ifndef RAILTYPEDATA_H
#define RAILTYPEDATA_H
#include <initializer_list>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

using namespace glm;

class RailTypeData {
public:
    constexpr static float FRICTION = 0.1f;
    constexpr static float GRAVITY = 2.5f;
    std::vector<vec2> pathPoints;
    ivec3 railBefore;
    ivec3 railAfter;
    float gravity;

    RailTypeData() = default;
    RailTypeData(ivec3 railBefore, ivec3 railAfter,std::initializer_list<vec2> pathPoints);

    static void placeRails(ivec3 railStart, ivec3 railEnd);
    static void getRailPlacements(ivec3 railStart, ivec3 railEnd);
};



#endif //RAILTYPEDATA_H
