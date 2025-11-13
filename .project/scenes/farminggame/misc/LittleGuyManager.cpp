//
// Created by drewg on 11/13/2025.
//

#include "LittleGuyManager.h"

FarmingWorld *LittleGuyManager::world = nullptr;

void LittleGuyManager::setWorld(FarmingWorld *world) {
    LittleGuyManager::world = world;
    LittleGuy::setWorld(world);
}

LittleGuyManager::LittleGuyManager() {
    guys = std::vector<LittleGuy*>();
}

LittleGuyManager::~LittleGuyManager() {
    cleanup();
}

void LittleGuyManager::update(const float dt) {
    for (const auto guy : guys) {
        guy->update(dt);
    }
}

void LittleGuyManager::draw(const bool bind) const {
    for (const auto guy : guys) {
        guy->draw(bind);
    }
}

void LittleGuyManager::cleanup() {
    for (const auto guy : guys) {
        guy->clearObjects();
        delete guy;
    }
    guys.clear();
}

int LittleGuyManager::setGoal(const FarmingObject::TypeID goal, const int amount) {
    this->goalType = goal;
    this->goalAmount = amount;
    return createTasks(goal, amount);
}

bool LittleGuyManager::addGuy(LittleGuy *guy) {
    guys.push_back(guy);
    return true;
}

int LittleGuyManager::getCount() const {
    return static_cast<int>(guys.size());
}

int LittleGuyManager::createTasks(const FarmingObject::TypeID type, const int amount) {
    int amountNeeded = amount;
    bool done = amountNeeded == 0;
    //if unused guys have items needed
    for (int i = 0; i < guys.size(); i++) {
        if (!guys[i]->isBeingUsed()) {
            const int amountGuyHas = guys[i]->hasItem(type);
            const int amountToUse = min(amountGuyHas, amountNeeded);
            amountNeeded -= amountToUse;
            // "deliver" task = (travel to barn + deposit item * amountToUse)
            //make sure to set guy as being used
        }
    }
    //if item is on ground
    for (int i = 0; i < world->items.size(); i++) {
        if (!world->items[i]->isBeingUsed() && world->items[i]->getType() == type) {
            world->items[i]->setBeingUsed(true);
            //"fetch" task = (travel to item + pickup + travel to barn + deposit item)
            //make sure to set guy as being used
        }
    }

    if (FarmingObject::getData<ItemProduce::ProduceData>(type)) {
        const ItemProduce::ProduceData *produceData = FarmingObject::getData<ItemProduce::ProduceData>(type);
        //search for unused crops (sort by travel time + time until fertile for priority)
        //std::vector<ivec2> cropTiles = findTiles(type.crop, amountNeeded)
        //const int amountToHarvest = cropTiles.size();
        //amountNeeded -= amountToHarvest;
        //"harvest" task = (travel to plant + harvest + pickup + travel to barn + deposit item) add with delay of
        if (amountNeeded > 0) {//not enough crops available
            const TilePlant::PlantData *plantData = FarmingObject::getData<TilePlant::PlantData>(produceData->producedFrom);
            const int seedsAvailable = createTasks(plantData->seed, amountNeeded);

            if (seedsAvailable < amountNeeded) return amount - amountNeeded;
            if (amountNeeded > 0) {
                //std::vector<ivec2> farmland = findFloor(FarmingWorld::FARMLAND, seedsAvailable);
                //if (farmland.size() < amountNeeded) return amount - amountNeeded;
                amountNeeded -= seedsAvailable;
                /*
                for(int i = 0; i < farmland.size(); i++) {
                    //"plant" task = (travel to farmland tile) + plant seed)
                    //"harvest" task = (travel to plant + harvest + pickup + travel to barn + deposit item) add with delay of "plant".cost() + plantData->ticksBetweenStage * plantData->ripeStage
                }


                */

            }
        }
    }

    //if its produce
    // - if crop available
    //   - add travel time + time until fertile for priority
    // - if NO crop available,
    //   - int seedsAvailable = createTasks(type.seed, amountNeeded)
    //   - if(seedsAvailable < amountNeeded) return amount - amountNeeded;
    //   - std::vector<ivec2> farmlandTiles = findFloor(type.crop, seedsAvailable);
    //   - if(farmlandTiles.size() < amountNeeded) return amount - amountNeeded;
    //   - for(int i = 0; i < farmlandTiles.size(); i++) plant/grow seeds
    //     - plant seeds
    //     - somehow wait seedGrowthTile
    //     - harvest plant



    return amount - amountNeeded;
}
