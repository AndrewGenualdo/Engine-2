//
// Created by cobble on 11/5/2025.
//

#include "FarmingObject.h"

#include "farmingWorld.h"
#include "items/produce/ItemProduce.h"
#include "items/seeds/ItemSeed.h"

std::map<FarmingObject::TypeID, FarmingObject::ObjectData *> FarmingObject::objectData = std::map <TypeID, ObjectData *>();
FarmingWorld *FarmingObject::world = nullptr;

FarmingObject::ObjectData::~ObjectData() = default;

void FarmingObject::loadData() {
    objectData[TypeID::ITEM] = new ObjectData(TypeID::ITEM, TypeID::NONE);


    objectData[TypeID::ITEM_PRODUCE] = new Item::ItemData(TypeID::ITEM_PRODUCE, TypeID::ITEM, 63);

    objectData[TypeID::ITEM_PRODUCE_TOMATO] = new ItemProduce::ProduceData(TypeID::ITEM_PRODUCE_TOMATO, TypeID::ITEM_PRODUCE, TypeID::TILE_PLANT_TOMATO, 1);
    objectData[TypeID::ITEM_PRODUCE_CARROT] = new ItemProduce::ProduceData(TypeID::ITEM_PRODUCE_CARROT, TypeID::ITEM_PRODUCE, TypeID::TILE_PLANT_CARROT, 3);


    objectData[TypeID::ITEM_SEED] = new Item::ItemData(TypeID::ITEM_SEED, TypeID::ITEM, 63);

    objectData[TypeID::ITEM_SEED_TOMATO] = new ItemSeed::SeedData(TypeID::ITEM_SEED_TOMATO, TypeID::ITEM_SEED, TypeID::ITEM_PRODUCE_TOMATO, 0);
    objectData[TypeID::ITEM_SEED_CARROT] = new ItemSeed::SeedData(TypeID::ITEM_SEED_CARROT, TypeID::ITEM_SEED, TypeID::ITEM_PRODUCE_CARROT, 2);



    objectData[TypeID::TILE] = new ObjectData(TypeID::TILE, TypeID::NONE);


    objectData[TypeID::TILE_PLANT] = new Tile::TileData(TypeID::TILE_PLANT, TypeID::TILE);

    objectData[TypeID::TILE_PLANT_TOMATO] = new TilePlant::PlantData(TypeID::TILE_PLANT_TOMATO, TypeID::TILE_PLANT, TypeID::ITEM_PRODUCE_TOMATO, 4, 4, 25, 50, 1);
    objectData[TypeID::TILE_PLANT_CARROT] = new TilePlant::PlantData(TypeID::TILE_PLANT_CARROT, TypeID::TILE_PLANT, TypeID::ITEM_PRODUCE_CARROT, 4, 4, 50, 50, 5);
}

void FarmingObject::cleanData() {
    for (auto&[fst, snd] : objectData) {
        delete snd;
    }
    objectData.clear();
}

void FarmingObject::setWorld(FarmingWorld *world) {
    FarmingObject::world = world;
}

FarmingObject::FarmingObject(const ivec2 tile) {
    this->tile = tile;
    this->beingUsed = false;
}

FarmingObject::~FarmingObject() = default;

void FarmingObject::update(float deltaTime) {
}

void FarmingObject::tick() {
}

void FarmingObject::draw(bool bind) {
}

void FarmingObject::draw(float offsetX, float offsetY, bool bind) {
}

FarmingObject::TypeID FarmingObject::getType() const {
    return TypeID::NONE;
}

bool FarmingObject::destroy() {
    if (world != nullptr) {
        for (int i = 0; i < world->objects.size(); i++) {
            if (world->objects[i] == this) {
                delete world->objects[i];
                world->objects.erase(world->objects.begin() + i);
                return true;
            }
        }
    }
    return false;
}

void FarmingObject::setBeingUsed(const bool isBeingUsed) {
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

