//
// Created by cobble on 11/19/2025.
//

#ifndef ENGINE_2_TILEPLANTBLUEBERRY_H
#define ENGINE_2_TILEPLANTBLUEBERRY_H
#include "TilePlant.h"


class TilePlantBlueberry : public TilePlant {
public:
    TilePlantBlueberry() = default;
    explicit TilePlantBlueberry(ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_TILEPLANTBLUEBERRY_H