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
    this->itemPromise = nullptr;
}

TaskHarvestPlant::TaskHarvestPlant(LittleGuy *guy, FarmingObject::TypeID type) {
    this->guy = guy;
    this->ticksUntilHarvest = FarmingObject::getData<TilePlant::PlantData>(type)->ticksToHarvest;
    this->plant = nullptr;
    this->itemPromise = nullptr;
}

TaskHarvestPlant::~TaskHarvestPlant() {

};

bool TaskHarvestPlant::tick() {
    if (plant != nullptr) {
        ticksUntilHarvest--;
        if (ticksUntilHarvest <= 0) {
            itemPromise = world->createItem(FarmingObject::getData<TilePlant::PlantData>(plant->getType())->produces, plant->tile);
            plant->destroy();
            return true;
        }
        return false;
    }
    return true;
}

std::string TaskHarvestPlant::getName() {
    std::string out = "Harvest Plant: {";
    if (plant == nullptr) out += "NULL";
    else out += std::to_string(plant->tile.x) + ", " + std::to_string(plant->tile.y) + " ; " + std::to_string(static_cast<int>(plant->getType()));
    out +="} ";
    return out;
}

float TaskHarvestPlant::getCost() {
    return ticksUntilHarvest;
}

void TaskHarvestPlant::clear() {
    if (plant != nullptr) plant->setBeingUsed(false);
    if (itemPromise != nullptr) itemPromise->setBeingUsed(false);
}

void TaskHarvestPlant::setPlant(TilePlant *plant) {
    this->plant = plant;
}

Item * TaskHarvestPlant::getResultItem() const {
    return itemPromise;
}
