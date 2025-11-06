//
// Created by drewg on 11/5/2025.
//

#ifndef ENGINE_2_TILETOMATOPLANT_H
#define ENGINE_2_TILETOMATOPLANT_H

#include "../Tile.h"

class FarmingWorld;
class TilePlant : public Tile {
protected:
    static FarmingWorld *world;

public:
    static void setWorld(FarmingWorld *world);

    enum PlantType {
        TOMATO
    };

    class PlantData : public TileData {
    public:
        PlantType plantType;

        explicit PlantData(const PlantType plantType) : TileData(PLANT) {
            this->plantType = plantType;
        }
    };

    int stage = 0;
    int stageCount = 1;
    int ticksBetweenStage = 1;
    int ticksUntilStage = 1;
    int ripeStage = 1;

    TilePlant() = default;
    TilePlant(ObjectType type, ivec2 tile, int stageCount, int ticksBetweenStage, int ripeStage);

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


#endif //ENGINE_2_TILETOMATOPLANT_H
