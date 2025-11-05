//
// Created by drewg on 11/5/2025.
//

#include "TilePlant.h"

#include "../farmingWorld.h"

FarmingWorld *TilePlant::world = nullptr;

void TilePlant::setWorld(FarmingWorld *world) {
    TilePlant::world = world;
}

TilePlant::TilePlant(const PlantType type, const ivec2 tile, const int stageCount, const int ticksBetweenStage, const int ripeStage) : Tile(tile) {
    this->type = type;
    this->stage = 0;
    this->ticksBetweenStage = ticksBetweenStage;
    this->ticksUntilStage = ticksBetweenStage;
    this->stageCount = stageCount;
    this->ripeStage = ripeStage - 1;
}

void TilePlant::update(float dt) {

}

void TilePlant::tick() {
    if (stage == stageCount - 1) return;
    ticksUntilStage--;
    if (ticksUntilStage <= 0) {
        stage++;
        ticksUntilStage = ticksBetweenStage;
        updatePlantState();
    }
    if (stage >= stageCount) stage = stageCount - 1;
}

void TilePlant::draw(bool bind) {

}

std::string TilePlant::getConfigKey() {
    return Tile::getConfigKey() + "_PLANT";
}

std::string TilePlant::getConfig() {
    return Tile::getConfig() + std::to_string(type) + " " + std::to_string(stage) + " " + std::to_string(stageCount) + " " + std::to_string(ticksBetweenStage) + " " + std::to_string(ticksUntilStage) + " " + std::to_string(ripeStage) + "\n";
}

void TilePlant::loadConfig(const std::string &line, int i) {
    if (i == 2) {
        std::istringstream iss(line);
        int value;
        iss >> value >> stage >> stageCount >> ticksBetweenStage >> ticksUntilStage >> ripeStage;
        type = static_cast<PlantType>(value);
    } else Tile::loadConfig(line, i);
}

bool TilePlant::isRipe() const {
    return stage == ripeStage;
}

void TilePlant::updatePlantState() {
    world->plantData[tile.y * FarmingWorld::TILES_HORIZ + tile.x] = getTextureOffset() + stage;
}

unsigned int TilePlant::getTextureOffset() {
    return 0;
}
