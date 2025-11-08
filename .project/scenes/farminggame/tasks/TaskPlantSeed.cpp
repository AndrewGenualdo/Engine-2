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
}

TaskPlantSeed::~TaskPlantSeed() = default;

bool TaskPlantSeed::tick() {
    for (int i = 0; i < world->objects.size(); i++) {
        if (dynamic_cast<Tile*>(world->objects[i]) && dynamic_cast<Tile*>(world->objects[i])->tile == tile) {
            return true;
        }
    }
    if (guy->takeItem(FarmingObject::getData<TilePlant::PlantData>(plantType)->seed)) {
        Tile *newPlant = FarmingWorld::createTile(plantType, tile);
        if (newPlant != nullptr) {
            newPlant->setBeingUsed(true);
            plantProimse = dynamic_cast<TilePlant*>(newPlant);
            world->objects.push_back(newPlant);
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

void TaskPlantSeed::clear() {

}

TilePlant * TaskPlantSeed::getResultPlant() const {
    return plantProimse;
}
