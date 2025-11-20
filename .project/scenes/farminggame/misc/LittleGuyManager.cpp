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
    tasks = std::vector<std::pair<int, std::vector<Task *>>>();
}

LittleGuyManager::~LittleGuyManager() {
    cleanup();
}

void LittleGuyManager::update(const float dt) {
    /*for (const auto guy : guys) {
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
    }*/
    for (auto &[delay, guysTasks] : tasks) {
        if (delay == 0) {
            if (!guysTasks.empty()) {
                if (guysTasks[0]->getGuy() == nullptr) continue;
                if (!(dynamic_cast<TaskTravel*> (guysTasks[0]))) continue;
                if (guysTasks[0]->update(dt)) {
                    guysTasks[0]->setActive(false);
                    delete guysTasks[0];  //delete completed task
                    guysTasks.erase(guysTasks.begin()); //remove from task list for this guy
                    //if (!guysTasks.empty()) guysTasks[0]->getGuy()->setTask(guysTasks[0]); //set guys next task if there is one available
                    break;
                }
            }
        }
    }
}

void LittleGuyManager::tick() {
    if (world->logs) {
        std::cout << "------------------------------------------------------------------------" << std::endl;
        for (int i = 0; i < tasks.size(); i++) {
            std::cout << "=====================================" << std::endl;
            for (int j = 0; j < tasks[i].second.size(); j++) {
                std::cout << "[" << tasks[i].first << "] [" << (tasks[i].second[j]->getGuy() != nullptr ? "true" : "false") << "] " << tasks[i].second[j]->getName() << std::endl;
            }
        }
        std::cout << "=====================================" << std::endl;
    }


    for (auto &[delay, guysTasks] : tasks) {
        if (delay == 0) {
            if (!guysTasks.empty()) {
                if (guysTasks[0]->getGuy() == nullptr) continue;
                if (dynamic_cast<TaskTravel*> (guysTasks[0])) continue;
                if (guysTasks[0]->tick()) {
                    if (dynamic_cast<TaskPlantSeed*>(guysTasks[0])) {
                        TilePlant *plant = dynamic_cast<TaskPlantSeed*>(guysTasks[0])->getResultPlant();
                        //search for harvest task on same tile and set a real delay (rather than indefinite stall)
                        bool found = false;
                        for (auto &[plantGrowthTime, harvestTaskSearch] : tasks) {
                            bool foundTravel = false;
                            for (const auto & harvestTask : harvestTaskSearch) {
                                if (!foundTravel) {
                                    if (dynamic_cast<TaskTravel*>(harvestTask)) {
                                        foundTravel = true;
                                        if (dynamic_cast<TaskTravel*>(harvestTask)->goal != plant->tile) break;
                                        continue;
                                    }
                                }
                                if (dynamic_cast<TaskHarvestPlant*>(harvestTask) && dynamic_cast<TaskHarvestPlant*>(harvestTask)->getPlant() == nullptr) {
                                    dynamic_cast<TaskHarvestPlant*>(harvestTask)->setPlant(plant);
                                    const auto plantData = FarmingObject::getData<TilePlant::PlantData>(plant->getType());
                                    plantGrowthTime = plantData->ticksBetweenStage * (plantData->ripeStage);
                                    found = true;
                                    break;
                                }
                            }
                            if (found) break;
                        }
                    } else if (dynamic_cast<TaskHarvestPlant*>(guysTasks[0])) {
                        for (const auto & pickupTask : guysTasks) {
                            if (dynamic_cast<TaskPickupItem*>(pickupTask)) {
                                dynamic_cast<TaskPickupItem*>(pickupTask)->setItem(dynamic_cast<TaskHarvestPlant*>(guysTasks[0])->getResultItem());
                                break;
                            }
                        }
                    }

                    guysTasks[0]->setActive(false);
                    if (guysTasks.size() == 1) guysTasks[0]->getGuy()->setBeingUsed(false);
                    delete guysTasks[0];  //delete completed task
                    guysTasks.erase(guysTasks.begin()); //remove from task list for this guy
                    //if (!guysTasks.empty()) guysTasks[0]->getGuy()->setTask(guysTasks[0]); //set guys next task if there is one available
                    if (!guysTasks.empty() && dynamic_cast<TaskTravel*>(guysTasks[0])) {
                        dynamic_cast<TaskTravel*>(guysTasks[0])->generatePath();
                    }
                    break;
                }
            }
        }
    }

    /*for (const auto guy : guys) {
        guy->tick();
        if (guy->getTask() == nullptr || dynamic_cast<TaskTravel *> (guy->getTask())) continue; //skip all tasks types that use update() instead of tick()
        if (guy->getTask()->tick()) { //if the current task is completed
            for (auto &[delay, guysTasks] : tasks) { //loop through all guys tasks to find its tasks
                if (delay == 0) { //make sure task is available and not on a delay
                    if (!guysTasks.empty() && guysTasks[0]->getGuy() == guy) { //if the tasks are this guys tasks

                    }
                }
            }
        }

    }*/
    for (int i = tasks.size() - 1; i >= 0; i--) {
        if (tasks[i].second.empty()) {
            tasks.erase(tasks.begin() + i);
            continue;
        }
        //std::cout << tasks.size() << std::endl;
        if (tasks[i].first == 0 && tasks[i].second[0]->getGuy() == nullptr) { //check if task needs a guy
            for (auto & guy : guys) {
                if (!guy->isBeingUsed()) {
                    for (const auto & task : tasks[i].second) {
                        task->setGuy(guy);
                    }
                    if (!tasks[i].second.empty() && dynamic_cast<TaskTravel*>(tasks[i].second[0])) {
                        dynamic_cast<TaskTravel*>(tasks[i].second[0])->generatePath();
                    }
                    guy->setBeingUsed(true);
                    break;
                }
            }
        }
    }
    for (auto &[delay, guysTasks] : tasks) {
        if (delay > 0) delay--;
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

void LittleGuyManager::clear() {
    for (auto &[delay, guysTasks] : tasks) {
        for (auto & task : guysTasks) {
            task->setActive(false);
            delete task;
            task = nullptr;
        }
        guysTasks.clear();
    }
    tasks.clear();
    for (auto & guy : guys) {
        delete guy;
        guy = nullptr;
    }
    guys.clear();
}

std::string LittleGuyManager::getConfig() const {
    std::string output;
    output += "LITTLE_GUYS\n";
    for (auto guy : guys) {
        output += "LITTLE_GUY_START\n";
        output += guy->getConfig();
        output += "LITTLE_GUY_END\n";
    }
    return output;
}

int LittleGuyManager::setGoal(const FarmingObject::TypeID goal, const int amount) {
    this->goalType = goal;
    this->goalAmount = amount;

    for (auto &[delay, guysTasks] : tasks) {
        for (auto & task : guysTasks) {
            task->setActive(false);
            if (task->getGuy() != nullptr) task->getGuy()->setBeingUsed(false);
            delete task;
            task = nullptr;
        }
        guysTasks.clear();
    }
    tasks.clear();
    int out = createTasks(goal, amount);
    for (int i = 0; i < tasks.size(); i++) {
        if (world->logs) {

            std::cout << "=====================================" << std::endl;
            for (int j = 0; j < tasks[i].second.size(); j++) {
                std::cout << "[" << tasks[i].first << "] " << tasks[i].second[j]->getName() << std::endl;
            }
            std::cout << "=====================================" << std::endl;
        }
        }

    return out;
}

bool LittleGuyManager::addGuy(LittleGuy *guy) {
    guys.push_back(guy);
    return true;
}

int LittleGuyManager::count() const {
    return static_cast<int>(guys.size());
}

int LittleGuyManager::createTasks(const FarmingObject::TypeID type, const int amount) {
    int amountNeeded = amount;
    if (amountNeeded <= 0) return amount;

    //if barn has enough of said item
    const int barnToUse = min(world->effectiveInventory[type], amountNeeded);
    if (barnToUse > 0) {
        amountNeeded -= barnToUse;
        if (amountNeeded <= 0) return amount;
    }


    //if unused guys have items needed
    for (const auto & guy : guys) {
        if (!guy->isBeingUsed()) {
            const int amountGuyHas = guy->hasItem(type);
            if (amountGuyHas > 0) {
                const int amountToUse = min(amountGuyHas, amountNeeded);
                std::vector<Task *> deliverTask; // "deliver" task = (travel to barn + deposit item * amountToUse)
                deliverTask.push_back(new TaskTravel(guy, FarmingWorld::INVENTORY_TILE));
                deliverTask.push_back(new TaskDepositItem(guy, type, amountToUse));

                tasks.emplace_back(0, deliverTask);
                guy->setBeingUsed(true);
                amountNeeded -= amountToUse;
                if (amountNeeded <= 0) return amount;
            }
        }
    }
    //if item is on ground
    for (const auto & item : world->items) {
        if (!item->isBeingUsed() && item->getType() == type) {
            std::vector<Task *> fetchTask; //"fetch" task = (travel to item + pickup + travel to barn + deposit item)
            fetchTask.push_back(new TaskTravel(nullptr, item->tile));
            fetchTask.push_back(new TaskPickupItem(nullptr, item));
            fetchTask.push_back(new TaskTravel(nullptr, item->tile, FarmingWorld::INVENTORY_TILE));
            fetchTask.push_back(new TaskDepositItem(nullptr, type, 1));
            tasks.emplace_back(0, fetchTask);
            item->setBeingUsed(true);
            amountNeeded--;
            if (amountNeeded <= 0) return amount;
        }
    }

    if (FarmingObject::getData<ItemProduce::ProduceData>(type)) {
        const ItemProduce::ProduceData *produceData = FarmingObject::getData<ItemProduce::ProduceData>(type);
        //search for unused crops (sort by travel time + time until fertile for priority)
        const std::vector<ivec2> cropTiles = world->getTiles(produceData->producedFrom, amountNeeded);
        const int amountToHarvest = static_cast<int>(cropTiles.size());
        for (const auto cropTile : cropTiles) {
            std::vector<Task *> harvestTask; //"harvest" task = (travel to plant + harvest + pickup + travel to barn + deposit item) add with delay of plantData->ticksBetweenStage * (plantData->ripeStage - plant->stage) + plant->timeUntilNextStage
            harvestTask.push_back(new TaskTravel(nullptr, cropTile));
            harvestTask.push_back(new TaskHarvestPlant(nullptr, dynamic_cast<TilePlant*>(world->getTile(cropTile))));
            //only execute TaskPickupItem as soon as getResultItem() is NOT nullptr
            harvestTask.push_back(new TaskPickupItem(nullptr, dynamic_cast<TaskHarvestPlant *>(harvestTask[harvestTask.size()-1])->getResultItem()));
            harvestTask.push_back(new TaskTravel(nullptr, cropTile, FarmingWorld::INVENTORY_TILE));
            harvestTask.push_back(new TaskDepositItem(nullptr, type, 1));
            tasks.emplace_back(0, harvestTask);
        }
        amountNeeded -= amountToHarvest;
        if (amountNeeded <= 0) return amount;



        if (amountNeeded > 0) { //not enough crops available
            const TilePlant::PlantData *plantData = FarmingObject::getData<TilePlant::PlantData>(produceData->producedFrom);
            const int seedsAvailable = createTasks(plantData->seed, amountNeeded);

            if (seedsAvailable < amountNeeded) return amount - amountNeeded; //not enough available seeds
            if (amountNeeded > 0) { //leaving this in case I refactor and forget
                const std::vector<ivec2> farmland = world->getFarmland(seedsAvailable);
                if (farmland.size() < amountNeeded) return amount - amountNeeded; //not enough available farmland
                //std::vector<TilePlant*> tilePromises;
                for(const auto tile : farmland) {
                    std::vector<Task *> plantTask; //"plant" task = (travel to barn + withdraw seed + travel to farmland tile + plant seed)
                    plantTask.push_back(new TaskTravel(nullptr, FarmingWorld::INVENTORY_TILE));
                    plantTask.push_back(new TaskWithdrawItem(nullptr, plantData->seed, 1));
                    plantTask.push_back(new TaskTravel(nullptr, FarmingWorld::INVENTORY_TILE, tile));
                    plantTask.push_back(new TaskPlantSeed(nullptr, produceData->producedFrom, tile));
                    //tilePromises.push_back(dynamic_cast<TaskPlantSeed*>(plantTask[plantTask.size()-1])->getResultPlant());
                    tasks.emplace_back(0, plantTask); //relies on barn having enough seeds condition
                }
                for (const auto tile : farmland) {
                    std::vector<Task *> harvestTask; //"harvest" task = (travel to plant + harvest + pickup + travel to barn + deposit item) add with delay of plantData->ticksBetweenStage * (plantData->ripeStage - plant->stage) + plant->timeUntilNextStage
                    harvestTask.push_back(new TaskTravel(nullptr, tile));
                    //harvestTask.push_back(new TaskHarvestPlant(nullptr, tilePromises[i]));
                    harvestTask.push_back(new TaskHarvestPlant(nullptr, produceData->producedFrom));
                    //only execute TaskPickupItem as soon as getResultItem() is NOT nullptr
                    harvestTask.push_back(new TaskPickupItem(nullptr, nullptr /*dynamic_cast<TaskHarvestPlant *>(harvestTask[harvestTask.size()-1])->getResultItem()*/));
                    harvestTask.push_back(new TaskTravel(nullptr, tile, FarmingWorld::INVENTORY_TILE));
                    harvestTask.push_back(new TaskDepositItem(nullptr, type, 1));
                    tasks.emplace_back(-1, harvestTask); //gets set to a real value upon seed finished being planted
                }
                for (const auto tile : farmland) {
                    //"harvest" task = (travel to plant + harvest + pickup + travel to barn + deposit item) add with delay of "plant".cost() + plantData->ticksBetweenStage * (plantData->ripeStage + 1)

                }
                amountNeeded -= seedsAvailable;
                if (amountNeeded <= 0) return amount;
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
