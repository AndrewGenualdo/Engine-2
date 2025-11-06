//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_TILEPLANTTOMATO_H
#define ENGINE_2_TILEPLANTTOMATO_H
#include "TilePlant.h"


class TilePlantTomato : public TilePlant {
public:
    TilePlantTomato() = default;
    explicit TilePlantTomato(ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
    unsigned int getTextureOffset() override;

};


#endif //ENGINE_2_TILEPLANTTOMATO_H