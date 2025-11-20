//
// Created by drewg on 11/5/2025.
//

#ifndef ENGINE_2_TILEPLANT_H
#define ENGINE_2_TILEPLANT_H

#include "../Tile.h"

class Item;
class FarmingWorld;
class TilePlant : public Tile {

public:

    class PlantData : public TileData {
    public:
        TypeID produces = TypeID::TILE_PLANT;
        TypeID seed = TypeID::ITEM_SEED;
        int stageCount = 1;
        int ripeStage = 1;
        int ticksBetweenStage = 1;
        int ticksToHarvest = 1;
        int amountProduces = 1;
        int textureIndex = 1;
        PlantData() : TileData() {}
        explicit PlantData(const std::string &configKey, const TypeID type, const TypeID parent, const TypeID produces, const TypeID seed, const int stageCount, const int ripeStage, const int ticksBetweenStage, const int ticksToHarvest, const int amountProduces, const int textureIndex) : TileData(configKey, type, parent) {
            this->produces = produces;
            this->seed = seed;
            this->stageCount = stageCount;
            this->ripeStage = ripeStage - 1;
            this->ticksBetweenStage = ticksBetweenStage;
            this->ticksToHarvest = ticksToHarvest;
            this->amountProduces = amountProduces;
            this->textureIndex = textureIndex;
        }
        ~PlantData() override = default;


    };


    int stage = 0;
    int ticksUntilStage = 1;
    int amount = 1;

    TilePlant() = default;
    TilePlant(TypeID type, ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

    [[nodiscard]] bool isRipe() const;
    Item* harvest();

    bool destroy() override;
    virtual void updatePlantState();

};


#endif //ENGINE_2_TILEPLANT_H
