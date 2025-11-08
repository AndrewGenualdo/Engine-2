//
// Created by drewg on 11/5/2025.
//

#ifndef ENGINE_2_TILEPLANT_H
#define ENGINE_2_TILEPLANT_H

#include "../Tile.h"

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
        int textureIndex = 1;
        PlantData() : TileData() {}
        explicit PlantData(const TypeID type, const TypeID parent, const TypeID produces, const TypeID seed, const int stageCount, const int ripeStage, const int ticksBetweenStage, const int ticksToHarvest, const int textureIndex) : TileData(type, parent) {
            this->produces = produces;
            this->seed = seed;
            this->stageCount = stageCount;
            this->ripeStage = ripeStage - 1;
            this->ticksBetweenStage = ticksBetweenStage;
            this->ticksToHarvest = ticksToHarvest;
            this->textureIndex = textureIndex;
        }
        ~PlantData() override = default;


    };


    int stage = 0;
    int ticksUntilStage = 1;

    TilePlant() = default;
    TilePlant(TypeID type, ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

    [[nodiscard]] bool isRipe() const;

    bool destroy() override;
    virtual void updatePlantState();

};


#endif //ENGINE_2_TILEPLANT_H
