//
// Created by cobble on 8/29/2025.
//

#ifndef RAILTYPEDATA_H
#define RAILTYPEDATA_H
#include <initializer_list>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class RailTypeData {
public:
    constexpr static float FRICTION = 0.1f;
    constexpr static float GRAVITY = 2.5f;
    std::vector<glm::vec2> pathPoints;
    glm::ivec3 railBefore;
    glm::ivec3 railAfter;
    float gravity;

    RailTypeData() = default;
    RailTypeData(glm::ivec3 railBefore, glm::ivec3 railAfter,std::initializer_list<glm::vec2> pathPoints);
};



#endif //RAILTYPEDATA_H
