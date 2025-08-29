//
// Created by cobble on 8/29/2025.
//

#include "railTypeData.h"



RailTypeData::RailTypeData(glm::vec3 railBefore, glm::vec3 railAfter, std::initializer_list<glm::vec2> pathPoints)  {
    this->pathPoints = std::vector<glm::vec2>();
    for(glm::vec2 point : pathPoints) this->pathPoints.push_back(point);
    this->railBefore = railBefore;
    this->railAfter = railAfter;
    this->gravity = abs(railBefore.y - railAfter.y) * GRAVITY;
}
