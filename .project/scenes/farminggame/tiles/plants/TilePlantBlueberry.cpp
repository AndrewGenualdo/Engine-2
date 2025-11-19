//
// Created by cobble on 11/19/2025.
//

#include "TilePlantBlueberry.h"

TilePlantBlueberry::TilePlantBlueberry(ivec2 tile) : TilePlant(TypeID::TILE_PLANT_BLUEBERRY, tile) {
    TilePlantBlueberry::updatePlantState();
}

void TilePlantBlueberry::update(float dt) {

}

void TilePlantBlueberry::tick() {
    TilePlant::tick();
}

void TilePlantBlueberry::draw(bool bind) {

}

FarmingObject::TypeID TilePlantBlueberry::getType() const {
    return TypeID::TILE_PLANT_BLUEBERRY;
}

std::string TilePlantBlueberry::getConfig() {
    return TilePlant::getConfig() + "-\n";
}

void TilePlantBlueberry::loadConfig(const std::string &line, int i) {
    if (i == 3) {
        TilePlantBlueberry::updatePlantState(); //this one happens last, after all other data has been loaded
    } else TilePlant::loadConfig(line, i);
}
