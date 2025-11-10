//
// Created by andrew.genualdo on 11/7/2025.
//

#ifndef ENGINE_2_TILEPLANTCARROT_H
#define ENGINE_2_TILEPLANTCARROT_H
#include "TilePlant.h"


class TilePlantCarrot : public TilePlant {
public:
    TilePlantCarrot() = default;
    explicit TilePlantCarrot(ivec2 tile);

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_TILEPLANTCARROT_H