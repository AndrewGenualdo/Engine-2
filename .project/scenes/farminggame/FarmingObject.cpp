//
// Created by cobble on 11/5/2025.
//

#include "FarmingObject.h"

#include "items/produce/ItemProduce.h"
#include "items/seeds/ItemSeed.h"

void FarmingObject::loadData() {
    objectData.push_back(new ItemProduce::ProduceData(ItemProduce::TOMATO));
    objectData.push_back(new ItemProduce::ProduceData(ItemProduce::CARROT));
    objectData.push_back(new ItemSeed::SeedData(ItemSeed::TOMATO));
    objectData.push_back(new ItemSeed::SeedData(ItemSeed::CARROT));
}

FarmingObject::FarmingObject(const ObjectType type, const ivec2 tile) {
    this->type = type;
    this->tile = tile;
    this->beingUsed = false;
}

void FarmingObject::update(float deltaTime) {
}

void FarmingObject::tick() {
}

void FarmingObject::draw(bool bind) {
}

void FarmingObject::draw(float offsetX, float offsetY, bool bind) {
}

void FarmingObject::setUsed(const bool isBeingUsed) {
    this->beingUsed = isBeingUsed;
}

bool FarmingObject::isBeingUsed() const {
    return beingUsed;
}

std::string FarmingObject::getConfigKey() {
    return "";
}

std::string FarmingObject::getConfig() {
    return std::to_string(tile.x) + " " + std::to_string(tile.y) + "\n";
}

void FarmingObject::loadConfig(const std::string &line, int i) {
    std::istringstream iss(line);
    iss >> tile.x >> tile.y;
}
