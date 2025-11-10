//
// Created by cobble on 11/8/2025.
//

#ifndef ENGINE_2_TASKPLANTSEED_H
#define ENGINE_2_TASKPLANTSEED_H
#include "Task.h"


class TaskPlantSeed : public Task {

    FarmingObject::TypeID plantType = FarmingObject::TypeID::ITEM_SEED;
    ivec2 tile;
    TilePlant *plantProimse = nullptr;

public:
    TaskPlantSeed(LittleGuy *guy, FarmingObject::TypeID plantType, ivec2 tile);
    ~TaskPlantSeed() override;

    bool tick() override;
    std::string getName() override;
    float getCost() override;
    void setActive(bool active) override;

    TilePlant* getResultPlant() const;
};


#endif //ENGINE_2_TASKPLANTSEED_H