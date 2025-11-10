//
// Created by cobble on 11/8/2025.
//

#include "TaskPlantSeed.h"
#include "../littleGuy.h"

TaskPlantSeed::TaskPlantSeed(LittleGuy *guy, FarmingObject::TypeID plantType, ivec2 tile) {
    this->guy = guy;
    this->plantType = plantType;
    this->tile = tile;
    this->plantProimse = nullptr;
    world->getTile(tile)->setBeingUsed(true);
}

TaskPlantSeed::~TaskPlantSeed() = default;

bool TaskPlantSeed::tick() {

    bool exists = world->getTile(tile)->exists();

    if(exists) {
        return true;
    }

    if (guy->takeItem(FarmingObject::getData<TilePlant::PlantData>(plantType)->seed)) {
        Tile *newPlant = FarmingWorld::createTile(plantType, tile);
        if (newPlant != nullptr) {
            newPlant->setBeingUsed(true);
            plantProimse = dynamic_cast<TilePlant*>(newPlant);
            //world->setTile(tile, newPlant);
            world->updateTile(tile, newPlant);
            return false;
        }
    }
    return true;
}

std::string TaskPlantSeed::getName() {
    return "Plant Seed: {" + std::to_string(static_cast<int>(plantType)) + ": " + std::to_string(tile.x) + ", " + std::to_string(tile.y) + "} ";
}

float TaskPlantSeed::getCost() {
    return 1;
}

void TaskPlantSeed::setActive(bool active) {

}

TilePlant * TaskPlantSeed::getResultPlant() const {
    return plantProimse;
}
