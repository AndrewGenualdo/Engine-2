//
// Created by cobble on 11/6/2025.
//

#include "TaskHarvestPlant.h"
#include "../littleGuy.h"

TaskHarvestPlant::TaskHarvestPlant(LittleGuy *guy, TilePlant *plant) {
    this->guy = guy;
    this->ticksUntilHarvest = FarmingObject::getData<TilePlant::PlantData>(plant->getType())->ticksToHarvest;
    this->plant = plant;
    this->plant->setBeingUsed(true);
}

TaskHarvestPlant::~TaskHarvestPlant() = default;

bool TaskHarvestPlant::tick() {
    if (plant != nullptr) {
        ticksUntilHarvest--;
        if (ticksUntilHarvest <= 0) {
            item = world->createItem(FarmingObject::getData<TilePlant::PlantData>(plant->getType())->produces, plant->tile);
            plant->destroy();
            return true;
        }
        return false;
    }
    return true;
}

std::string TaskHarvestPlant::getName() {
    return "Harvest Plant";
}

float TaskHarvestPlant::getCost() {
    return ticksUntilHarvest;
}

Item * TaskHarvestPlant::getResultItem() const {
    return item;
}
