//
// Created by cobble on 11/6/2025.
//

#ifndef ENGINE_2_TASKHARVESTPLANT_H
#define ENGINE_2_TASKHARVESTPLANT_H
#include "Task.h"
#include "../tiles/plants/TilePlant.h"

class TaskHarvestPlant : public Task {

    constexpr static float HARVEST_DISTANCE = (FarmingWorld::TILE_WIDTH + FarmingWorld::TILE_HEIGHT) * 0.5f;

    int ticksUntilHarvest = 1;
    TilePlant *plant = nullptr;
    Item *item = nullptr;

public:
    TaskHarvestPlant(LittleGuy *guy, TilePlant *plant);
    ~TaskHarvestPlant() override;

    bool update(float dt) override;
    std::string getName() override;
    float getCost() override;

    Item* getResultItem() const;

};


#endif //ENGINE_2_TASKHARVESTPLANT_H