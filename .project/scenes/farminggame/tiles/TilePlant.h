//
// Created by drewg on 11/5/2025.
//

#ifndef ENGINE_2_TILETOMATOPLANT_H
#define ENGINE_2_TILETOMATOPLANT_H

#include "Tile.h"

class TilePlant : public Tile {

    enum PlantType {
        NONE,
        TOMATO
    };

    PlantType type;
    int stage;
    int stageCount;
    int ticksBetweenStage;
    int ticksUntilStage;
    int ripeStage;

    TilePlant(PlantType type, ivec2 tile, int stageCount, int ticksBetweenStage, int ripeStage);

    [[nodiscard]] bool isRipe() const;

};


#endif //ENGINE_2_TILETOMATOPLANT_H
