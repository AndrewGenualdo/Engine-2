//
// Created by cobble on 11/5/2025.
//

#include "TaskRetrieveItem.h"

#include "TaskPlantSeed.h"
#include "TaskWait.h"
#include "../items/Item.h"
#include "../littleGuy.h"
#include "../items/produce/ItemProduce.h"
#include "../tiles/plants/TilePlant.h"

TaskRetrieveItem::TaskRetrieveItem(LittleGuy *guy, FarmingObject::TypeID type, const int amount) : Task(guy) {
    this->goalAmount = amount;
    this->type = type;
    getClosestItem();
}

TaskRetrieveItem::~TaskRetrieveItem() {
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i] != nullptr) {
            delete tasks[i];
            tasks[i] = nullptr;
        }
    }
    tasks.clear();
}

bool TaskRetrieveItem::update(const float dt) {
    if (amount < goalAmount && !tasks.empty()) {
        if (dynamic_cast<TaskTravel*>(tasks[0])) {
            if (tasks[0]->update(dt)) {
                delete tasks[0];
                tasks[0] = nullptr;
                tasks.erase(tasks.begin());
            }
        }
        return false;
    }
    return true;
}

bool TaskRetrieveItem::tick() {
    if (amount < goalAmount && !tasks.empty()) { //make sure there is some way to get the thing
        bool del = false;
        if (dynamic_cast<TaskHarvestPlant*>(tasks[0])) { //harvest task
            if (tasks[0]->tick()) { //if complete, try to find the pickup task so it knows which one to pickup
                for (int i = 1; i < tasks.size(); i++) {
                    if (dynamic_cast<TaskPickupItem*>(tasks[i])) {
                        auto task = dynamic_cast<TaskHarvestPlant*>(tasks[0]);
                        task->setActive(false);
                        auto item = task->getResultItem();
                        dynamic_cast<TaskPickupItem*>(tasks[i])->setItem(item);
                        break;
                    }
                }
                del = true;
            }
        } else if (dynamic_cast<TaskPickupItem*>(tasks[0])) {
            if (tasks[0]->tick()) {
                amount++;
                if (amount < goalAmount) getClosestItem();
                del = true;
            }
        } else if (dynamic_cast<TaskWithdrawItem*>(tasks[0])) {
            if (tasks[0]->tick()) {
                int amountWithdrawn = goalAmount - dynamic_cast<TaskWithdrawItem*>(tasks[0])->getAmount();
                setActive(false);
                if (amountWithdrawn < goalAmount) return true; //make some better logic here later.
                del = true;
            }
        } else if (dynamic_cast<TaskWait*>(tasks[0])) {
            if (tasks[0]->tick()) {
                del = true;
            }
        } else if (dynamic_cast<TaskPlantSeed*>(tasks[0])) {
            if (tasks[0]->tick()) {
                for (int i = 1; i < tasks.size(); i++) {
                    if (dynamic_cast<TaskHarvestPlant*>(tasks[i])) {
                        auto plant = dynamic_cast<TaskPlantSeed*>(tasks[0])->getResultPlant();
                        dynamic_cast<TaskHarvestPlant*>(tasks[i])->setPlant(plant);
                        break;
                    }
                }
                del = true;
            }
        } else {
            return false;
        }
        if (del) {
            delete tasks[0];
            tasks[0] = nullptr;
            tasks.erase(tasks.begin());
        }
        return false;
    }
    return true;
}

std::string TaskRetrieveItem::getName() {
    std::string out = "Retrieve Item: {";
    for (auto & task : tasks) {
        out += task->getName();
    }
    out += "} ";
    return out;
}

float TaskRetrieveItem::getCost() {
    return Task::getCost();
}

void TaskRetrieveItem::setActive(bool active) {
    for (auto & task : tasks) {
        task->setActive(active);
    }
}

bool TaskRetrieveItem::getClosestItem() {

    std::vector<std::vector<Task*>> possibleTasks;

    Item *closestItem = nullptr;
    for (auto & item : world->items) {
        if (item && !item->isBeingUsed() && item->getType() == type) {
            if (closestItem == nullptr || length(closestItem->pos - guy->getPos()) > length(item->pos - guy->getPos())) {
                closestItem = item;
            }
        }
    }
    if (closestItem != nullptr) {
        possibleTasks.emplace_back();
        possibleTasks[possibleTasks.size()-1].push_back(new TaskTravel(guy, closestItem->tile));
        possibleTasks[possibleTasks.size()-1].push_back(new TaskPickupItem(guy, closestItem));
    }

    TilePlant *closestPlant = nullptr;
    if (FarmingObject::getData<ItemProduce::ItemData>(type)) { //make sure its a harvestable item
        for (int i = 0; i < FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT; i++) {
            auto plant = dynamic_cast<TilePlant*>(world->getTile(i)); //make sure the tile is a plant
            if (plant && !plant->isBeingUsed() && plant->isRipe() && plant->getType() == FarmingObject::getData<ItemProduce::ProduceData>(type)->producedFrom) {
                if (closestPlant == nullptr || length(FarmingWorld::getTilePos(closestPlant->tile.x, closestPlant->tile.y) - guy->getPos()) > length(FarmingWorld::getTilePos(plant->tile.x, plant->tile.y) - guy->getPos())) {
                    closestPlant = plant;
                }
            }
        }

        if (closestPlant != nullptr) { //harvest a plant for the item
            possibleTasks.emplace_back();
            possibleTasks[possibleTasks.size()-1].push_back(new TaskTravel(guy, closestPlant->tile));
            possibleTasks[possibleTasks.size()-1].push_back(new TaskHarvestPlant(guy, closestPlant));
            possibleTasks[possibleTasks.size()-1].push_back(new TaskPickupItem(guy, nullptr));
        } else { //grow a plant for the item

            //this is hilarious to me for some reason
            auto itemData = FarmingObject::getData<ItemProduce::ProduceData>(type);
            auto plantData = FarmingObject::getData<TilePlant::PlantData>(itemData->producedFrom);
            if (world->inventory[plantData->seed] > 0) {

                //REPLACE THIS SHITTY THING WITH A BETTER THING
                auto tileToPlant = ivec2(-1);

                for (int i = 0; i < FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT; i++) {
                    //bool isFarmland = true;//FarmingWorld::isFarmland(i);

                    //ADD A CHECK TO MAKE SURE THE TILE IS FARMLAND
                    if (!world->getTile(i)->exists() && !world->getTile(i)->isBeingUsed()) {
                        tileToPlant = ivec2(i % FarmingWorld::TILES_HORIZ, i / FarmingWorld::TILES_HORIZ);
                        break;
                    }
                }
                if (tileToPlant == ivec2(-1)) {
                    std::cout << "how" << std::endl;
                    return true;
                }

                possibleTasks.emplace_back();
                possibleTasks[possibleTasks.size()-1].push_back(new TaskTravel(guy, FarmingWorld::INVENTORY_TILE));
                possibleTasks[possibleTasks.size()-1].push_back(new TaskWithdrawItem(guy, plantData->seed, 1));
                possibleTasks[possibleTasks.size()-1].push_back(new TaskTravel(guy, FarmingWorld::INVENTORY_TILE, tileToPlant));//travel to plant spot
                possibleTasks[possibleTasks.size()-1].push_back(new TaskPlantSeed(guy, itemData->producedFrom, tileToPlant));//plant
                possibleTasks[possibleTasks.size()-1].push_back(new TaskWait(guy, plantData->ticksBetweenStage * plantData->ripeStage));//wait to grow? water? idk, might change this later somehow if I can think up a better system
                possibleTasks[possibleTasks.size()-1].push_back(new TaskHarvestPlant(guy, itemData->producedFrom)); //replace nullptr with promised plant from TaskPlantSeed
                possibleTasks[possibleTasks.size()-1].push_back(new TaskPickupItem(guy, nullptr));
            }
        }
    }

    int bestIndex = -1;
    float lowestCost = 0.0f;
    for (int i = 0; i < possibleTasks.size(); i++) {
        float cost = 0;
        for (auto & j : possibleTasks[i]) cost += j->getCost();

        if (bestIndex == -1 || cost < lowestCost) {
            bestIndex = i;
            lowestCost = cost;
        }
    }

    if (bestIndex != -1) {
        for (int i = 0; i < possibleTasks.size(); i++) {
            if (i != bestIndex) {
                for (auto & task : possibleTasks[i]) {
                    task->setActive(false);
                    delete task;
                    task = nullptr;
                }
                possibleTasks[i].clear();
            }
        }
        for (auto task : possibleTasks[bestIndex]) {
            task->setActive(true);
            tasks.push_back(task);
        }
    }

    return bestIndex == -1; //no available options
}

bool TaskRetrieveItem::getClosestPlant(FarmingObject::TypeID type) {
    return false;
}

ivec2 TaskRetrieveItem::getItemTile() const {
    return itemTile;
}
