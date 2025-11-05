//
// Created by drewg on 11/5/2025.
//

#include "TilePlant.h"

TilePlant::TilePlant(const PlantType type, const ivec2 tile, const int stageCount, const int ticksBetweenStage, const int ripeStage) : Tile(tile) {
    this->type = type;
    this->stage = 0;
    this->ticksBetweenStage = ticksBetweenStage;
    this->ticksUntilStage = ticksBetweenStage;
    this->stageCount = stageCount;
    this->ripeStage = ripeStage;
}

bool TilePlant::isRipe() const {
    return stage == ripeStage;
}
