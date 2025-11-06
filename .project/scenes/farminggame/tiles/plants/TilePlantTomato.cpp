//
// Created by cobble on 11/5/2025.
//

#include "TilePlantTomato.h"
#include "../../farmingWorld.h"

TilePlantTomato::TilePlantTomato(const ivec2 tile) : TilePlant(TOMATO, tile) {
    TilePlantTomato::updatePlantState();
}

void TilePlantTomato::update(float dt) {

}

void TilePlantTomato::tick() {
    TilePlant::tick();
}

void TilePlantTomato::draw(bool bind) {

}

std::string TilePlantTomato::getConfigKey() {
    return TilePlant::getConfigKey() + "_TOMATO";
}

std::string TilePlantTomato::getConfig() {
    return TilePlant::getConfig() + "-\n";
}

void TilePlantTomato::loadConfig(const std::string &line, int i) {
    if (i == 3) {
        TilePlantTomato::updatePlantState(); //this one happens last, after all other data has been loaded
    } else TilePlant::loadConfig(line, i);
}

unsigned int TilePlantTomato::getTextureOffset() {
    return 1;
}
