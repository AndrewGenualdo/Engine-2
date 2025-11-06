//
// Created by drewg on 11/5/2025.
//

#include "TilePlant.h"

#include "../../farmingWorld.h"

FarmingWorld *TilePlant::world = nullptr;

void TilePlant::setWorld(FarmingWorld *world) {
    TilePlant::world = world;
}

TilePlant::TilePlant(const PlantType plantType, const ivec2 tile) : Tile(PLANT, tile) {
    this->plantType = plantType;
    this->stage = 0;
    this->ticksUntilStage = 100;//ticksBetweenStage;
}

void TilePlant::update(float dt) {

}

void TilePlant::tick() {
    /*if (stage == stageCount - 1) return;
    ticksUntilStage--;
    if (ticksUntilStage <= 0) {
        stage++;
        ticksUntilStage = ticksBetweenStage;
        updatePlantState();
    }
    if (stage >= stageCount) stage = stageCount - 1;*/
}

void TilePlant::draw(bool bind) {

}

std::string TilePlant::getConfigKey() {
    return Tile::getConfigKey() + "_PLANT";
}

std::string TilePlant::getConfig() {
    return Tile::getConfig() + std::to_string(type) + " " + std::to_string(stage) + " " + std::to_string(ticksUntilStage) + "\n";
}

void TilePlant::loadConfig(const std::string &line, const int i) {
    if (i == 2) {
        std::istringstream iss(line);
        int value;
        iss >> value >> stage >> ticksUntilStage;
        type = static_cast<ObjectType>(value);
    } else Tile::loadConfig(line, i);
}

bool TilePlant::isRipe() const {
    return stage == 4;
    //return stage == ripeStage;
}

void TilePlant::updatePlantState() {
    if (world != nullptr) {
        world->plantData[tile.y * FarmingWorld::TILES_HORIZ + tile.x] = getTextureOffset() + stage;
    }
}

unsigned int TilePlant::getTextureOffset() {
    return 0;
}
