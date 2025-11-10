//
// Created by andrew.genualdo on 11/7/2025.
//

#include "TilePlantCarrot.h"
#include "../../farmingWorld.h"

TilePlantCarrot::TilePlantCarrot(const ivec2 tile) : TilePlant(TypeID::TILE_PLANT_CARROT, tile) {
    TilePlantCarrot::updatePlantState();
}

void TilePlantCarrot::update(float dt) {

}

void TilePlantCarrot::tick() {
    TilePlant::tick();
}

void TilePlantCarrot::draw(bool bind) {

}

FarmingObject::TypeID TilePlantCarrot::getType() const {
    return TypeID::TILE_PLANT_CARROT;
}

std::string TilePlantCarrot::getConfig() {
    return TilePlant::getConfig() + "-\n";
}

void TilePlantCarrot::loadConfig(const std::string &line, int i) {
    if (i == 3) {
        TilePlantCarrot::updatePlantState(); //this one happens last, after all other data has been loaded
    } else TilePlant::loadConfig(line, i);
}
