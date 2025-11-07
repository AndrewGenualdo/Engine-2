//
// Created by cobble on 11/5/2025.
//

#include "TaskRetrieveItem.h"
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
    if (travelTask != nullptr) {
        delete travelTask;
        travelTask = nullptr;
    }
    if (pickupTask != nullptr) {
        delete pickupTask;
        pickupTask = nullptr;
    }
    if (harvestTask != nullptr) {
        delete harvestTask;
        harvestTask = nullptr;
    }
}

bool TaskRetrieveItem::update(const float dt) {
    if (amount < goalAmount) {
        if (travelTask != nullptr) {
            if (travelTask->update(dt)) {
                delete travelTask;
                travelTask = nullptr;
            }
            return false;
        }
        if (harvestTask != nullptr) {
            if (harvestTask->update(dt)) {
                if (pickupTask != nullptr) {
                    Item *item = harvestTask->getResultItem();
                    pickupTask->setItem(item);
                }
                delete harvestTask;
                harvestTask = nullptr;
            }
            return false;
        }
        if (pickupTask != nullptr) {
            if (pickupTask->update(dt)) {
                delete pickupTask;
                pickupTask = nullptr;
                amount++;
                getClosestItem();
            }
            return false;
        }
    }

    return true;
}

std::string TaskRetrieveItem::getName() {
    return "Retrieve Item";
}

float TaskRetrieveItem::getCost() {
    return Task::getCost();
}

bool TaskRetrieveItem::getClosestItem() {


    Item *closestItem = nullptr;
    for (auto & object : world->objects) {
        Item *item = dynamic_cast<Item*>(object);
        if (item && !item->isBeingUsed() && item->getType() == type) {
            if (closestItem == nullptr || length(closestItem->pos - guy->getPos()) > length(item->pos - guy->getPos())) {
                closestItem = item;
            }
        }
    }

    TilePlant *closestPlant = nullptr;
    if (FarmingObject::getData<ItemProduce::ItemData>(type)) { //make sure its a harvestable item
        for (auto & object : world->objects) {
            TilePlant *plant = dynamic_cast<TilePlant *>(object);
            if (plant && !plant->isBeingUsed() && plant->isRipe() && plant->getType() == FarmingObject::getData<ItemProduce::ProduceData>(type)->producedFrom) {
                if (closestPlant == nullptr || length(FarmingWorld::getTilePos(closestPlant->tile.x, closestPlant->tile.y) - guy->getPos()) > length(FarmingWorld::getTilePos(plant->tile.x, plant->tile.y) - guy->getPos())) {
                    closestPlant = plant;
                }
            }
        }
    }


    TaskTravel* taskTravel = nullptr;
    float travelCost = 0;
    if (closestItem != nullptr) {
        taskTravel = new TaskTravel(guy, closestItem->tile);
        travelCost = taskTravel->getCost();
    }

    TaskTravel* taskTravelHarvest = nullptr;
    TaskHarvestPlant* taskHarvest = nullptr;
    float harvestCost = 0;
    if (closestPlant != nullptr) {
        taskTravelHarvest = new TaskTravel(guy, closestPlant->tile);
        taskHarvest = new TaskHarvestPlant(guy, closestPlant);
        harvestCost = taskHarvest->getCost() + taskTravelHarvest->getCost();
    }

    if (taskTravel != nullptr && taskHarvest != nullptr) {
        if (travelCost < harvestCost) {
            travelTask = taskTravel;
            delete taskHarvest;
            delete taskTravelHarvest;
            pickupTask = new TaskPickupItem(guy, closestItem);
        } else {
            travelTask = taskTravelHarvest;
            harvestTask = taskHarvest;
            delete taskTravel;
            pickupTask = new TaskPickupItem(guy, nullptr);
        }

        return false;
    }
    if (taskTravel != nullptr) {
        travelTask = taskTravel;
        pickupTask = new TaskPickupItem(guy, closestItem);
        return false;
    }
    if (taskHarvest != nullptr) {
        travelTask = taskTravelHarvest;
        harvestTask = taskHarvest;
        pickupTask = new TaskPickupItem(guy, nullptr);
        return false;
    }

    return true; //no available options
}
