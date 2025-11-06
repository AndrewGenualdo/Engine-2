//
// Created by drewg on 11/5/2025.
//

#ifndef ENGINE_2_TILEPLANT_H
#define ENGINE_2_TILEPLANT_H

#include "../Tile.h"

class FarmingWorld;
class TilePlant : public Tile {
protected:
    static FarmingWorld *world;

public:
    static void setWorld(FarmingWorld *world);

    enum PlantType {
        NONE,
        TOMATO,
        CARROT
    };

    class PlantData : public TileData {
    public:
        PlantType plantType;
        int stageCount = 1;
        int ripeStage = 1;
        int ticksBetweenStage = 1;
        explicit PlantData(const PlantType plantType, int stageCount, int ripeStage, int ticksBetweenStage) : TileData(PLANT) {
            this->stageCount = stageCount;
            this->ripeStage = ripeStage - 1;
            this->ticksBetweenStage = ticksBetweenStage;
            this->plantType = plantType;
        }
    };

    PlantType plantType = NONE;
    int stage = 0;
    int ticksUntilStage = 1;

    TilePlant() = default;
    TilePlant(PlantType plantType, ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

    [[nodiscard]] bool isRipe() const;

    virtual void updatePlantState();

    virtual unsigned int getTextureOffset();

};


#endif //ENGINE_2_TILEPLANT_H
