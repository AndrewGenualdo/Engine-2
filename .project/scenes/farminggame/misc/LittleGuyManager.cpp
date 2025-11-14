//
// Created by drewg on 11/13/2025.
//

#include "LittleGuyManager.h"

#include "../tasks/TaskDepositItem.h"
#include "../tasks/TaskHarvestPlant.h"
#include "../tasks/TaskTravel.h"
#include "../tasks/TaskPickupItem.h"
#include "../tasks/TaskPlantSeed.h"
#include "../tasks/TaskWithdrawItem.h"

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
        if (dynamic_cast<TaskTravel *> (guy->getTask())) { //only call update() on task types that use update() instead of tick()
            if (guy->getTask()->update(dt)) {
                for (auto &[delay, guysTasks] : tasks) { //loop through all guys tasks to find its tasks
                    if (delay == 0) { //make sure task is available and not on a delay
                        if (!guysTasks.empty() && guysTasks[0]->getGuy() == guy) { //if the tasks are this guys tasks
                            guysTasks[0]->setActive(false);
                            delete guysTasks[0];  //delete completed task
                            guysTasks.erase(guysTasks.begin()); //remove from task list for this guy
                            if (!guysTasks.empty()) guy->setTask(guysTasks[0]); //set guys next task if there is one available
                        }
                    }
                }
            }
        }
    }
}

void LittleGuyManager::tick() {
    for (const auto guy : guys) {
        guy->tick();
        if (dynamic_cast<TaskTravel *> (guy->getTask())) continue; //skip all tasks types that use update() instead of tick()
        if (guy->getTask()->tick()) { //if the current task is completed
            for (auto &[delay, guysTasks] : tasks) { //loop through all guys tasks to find its tasks
                if (delay == 0) { //make sure task is available and not on a delay
                    if (!guysTasks.empty() && guysTasks[0]->getGuy() == guy) { //if the tasks are this guys tasks
                        guysTasks[0]->setActive(false);
                        delete guysTasks[0];  //delete completed task
                        guysTasks.erase(guysTasks.begin()); //remove from task list for this guy
                        if (!guysTasks.empty()) guy->setTask(guysTasks[0]); //set guys next task if there is one available
                    }
                }
            }
        }

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
    //if barn has enough of said item
    if (const int barnToUse = min(world->effectiveInventory[type], amountNeeded) > 0) {
        amountNeeded -= barnToUse;
    }


    //if unused guys have items needed
    for (const auto & guy : guys) {
        if (!guy->isBeingUsed()) {
            const int amountGuyHas = guy->hasItem(type);
            const int amountToUse = min(amountGuyHas, amountNeeded);
            amountNeeded -= amountToUse;
            std::vector<Task *> deliverTask; // "deliver" task = (travel to barn + deposit item * amountToUse)
            deliverTask.push_back(new TaskTravel(guy, FarmingWorld::INVENTORY_TILE));
            deliverTask.push_back(new TaskDepositItem(guy, type, amountToUse));
            tasks.emplace_back(0, deliverTask);
            guy->setBeingUsed(true);
        }
    }
    //if item is on ground
    for (const auto & item : world->items) {
        if (!item->isBeingUsed() && item->getType() == type) {
            std::vector<Task *> fetchTask; //"fetch" task = (travel to item + pickup + travel to barn + deposit item)
            fetchTask.push_back(new TaskTravel(nullptr, item->tile));
            fetchTask.push_back(new TaskPickupItem(nullptr, item));
            fetchTask.push_back(new TaskTravel(nullptr, FarmingWorld::INVENTORY_TILE));
            fetchTask.push_back(new TaskDepositItem(nullptr, type, 1));
            tasks.emplace_back(0, fetchTask);
            item->setBeingUsed(true);

        }
    }

    if (FarmingObject::getData<ItemProduce::ProduceData>(type)) {
        const ItemProduce::ProduceData *produceData = FarmingObject::getData<ItemProduce::ProduceData>(type);
        //search for unused crops (sort by travel time + time until fertile for priority)
        const std::vector<ivec2> cropTiles = world->getTiles(produceData->producedFrom, amountNeeded);
        const int amountToHarvest = static_cast<int>(cropTiles.size());
        amountNeeded -= amountToHarvest;
        for (const auto cropTile : cropTiles) {
            std::vector<Task *> harvestTask; //"harvest" task = (travel to plant + harvest + pickup + travel to barn + deposit item) add with delay of plantData->ticksBetweenStage * (plantData->ripeStage - plant->stage) + plant->timeUntilNextStage
            harvestTask.push_back(new TaskTravel(nullptr, cropTile));
            harvestTask.push_back(new TaskHarvestPlant(nullptr, produceData->producedFrom));
            //only execute TaskPickupItem as soon as getResultItem() is NOT nullptr
            harvestTask.push_back(new TaskPickupItem(nullptr, dynamic_cast<TaskHarvestPlant *>(harvestTask[harvestTask.size()-1])->getResultItem()));
            harvestTask.push_back(new TaskTravel(nullptr, FarmingWorld::INVENTORY_TILE));
            harvestTask.push_back(new TaskDepositItem(nullptr, type, 1));
            tasks.emplace_back(0, harvestTask);
        }



        if (amountNeeded > 0) { //not enough crops available
            const TilePlant::PlantData *plantData = FarmingObject::getData<TilePlant::PlantData>(produceData->producedFrom);
            const int seedsAvailable = createTasks(plantData->seed, amountNeeded);

            if (seedsAvailable < amountNeeded) return amount - amountNeeded; //not enough available seeds
            if (amountNeeded > 0) { //leaving this in case I refactor and forget
                const std::vector<ivec2> farmland = world->getFarmland(seedsAvailable);
                if (farmland.size() < amountNeeded) return amount - amountNeeded; //not enough available farmland
                amountNeeded -= seedsAvailable;
                for(const auto tile : farmland) {
                    std::vector<Task *> plantTask; //"plant" task = (travel to barn + withdraw seed + travel to farmland tile + plant seed)
                    plantTask.push_back(new TaskTravel(nullptr, FarmingWorld::INVENTORY_TILE));
                    plantTask.push_back(new TaskWithdrawItem(nullptr, plantData->seed, 1));
                    plantTask.push_back(new TaskTravel(nullptr, tile));
                    plantTask.push_back(new TaskPlantSeed(nullptr, produceData->producedFrom, tile));
                    tasks.emplace_back(-1, plantTask); //relies on barn having enough seeds condition

                    //"harvest" task = (travel to plant + harvest + pickup + travel to barn + deposit item) add with delay of "plant".cost() + plantData->ticksBetweenStage * (plantData->ripeStage + 1)
                    std::vector<Task *> harvestTask; //"harvest" task = (travel to plant + harvest + pickup + travel to barn + deposit item) add with delay of plantData->ticksBetweenStage * (plantData->ripeStage - plant->stage) + plant->timeUntilNextStage
                    harvestTask.push_back(new TaskTravel(nullptr, tile));
                    harvestTask.push_back(new TaskHarvestPlant(nullptr, produceData->producedFrom));
                    //only execute TaskPickupItem as soon as getResultItem() is NOT nullptr
                    harvestTask.push_back(new TaskPickupItem(nullptr, dynamic_cast<TaskHarvestPlant *>(harvestTask[harvestTask.size()-1])->getResultItem()));
                    harvestTask.push_back(new TaskTravel(nullptr, FarmingWorld::INVENTORY_TILE));
                    harvestTask.push_back(new TaskDepositItem(nullptr, type, 1));
                    tasks.emplace_back(-1, harvestTask); //gets set to a real value upon seed finished being planted
                }


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
