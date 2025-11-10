//
// Created by drewg on 11/5/2025.
//

#include "TilePlant.h"

#include "../../farmingWorld.h"

TilePlant::TilePlant(const TypeID type, const ivec2 tile) : Tile(tile) {
    this->stage = 0;
    this->ticksUntilStage = getData<PlantData>(type)->ticksBetweenStage;
    tileExists = true;
}

void TilePlant::update(float dt) {

}

void TilePlant::tick() {
    auto* data = getData<PlantData>(getType());
    if (stage == data->stageCount - 1) return;
    ticksUntilStage--;
    if (ticksUntilStage == 0) {
        stage++;
        ticksUntilStage = data->ticksBetweenStage;
        updatePlantState();
    }
}

void TilePlant::draw(bool bind) {

}

FarmingObject::TypeID TilePlant::getType() const {
    return TypeID::TILE_PLANT;
}

std::string TilePlant::getConfig() {
    return Tile::getConfig() + std::to_string(stage) + " " + std::to_string(ticksUntilStage) + "\n";
}

void TilePlant::loadConfig(const std::string &line, const int i) {
    if (i == 2) {
        std::istringstream iss(line);
        iss >> stage >> ticksUntilStage;
    } else Tile::loadConfig(line, i);
}

bool TilePlant::isRipe() const {
    //if (stage == getData<PlantData>(getType())->ripeStage) std::cout << "ripe" << std::endl;
    return stage == getData<PlantData>(getType())->ripeStage;
}

bool TilePlant::destroy() {
    if (world != nullptr) {
        ivec2 t = tile;
        world->clearTile(tile);
        world->plantData[t.y * FarmingWorld::TILES_HORIZ + t.x] = FarmingWorld::EMPTY;
        return true;
    }
    return false;
}

void TilePlant::updatePlantState() {
    if (world != nullptr) {
        world->plantData[tile.y * FarmingWorld::TILES_HORIZ + tile.x] = getData<PlantData>(getType())->textureIndex + stage;
    }
}


