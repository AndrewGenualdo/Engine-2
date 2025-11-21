//
// Created by andrew.genualdo on 11/21/2025.
//

#ifndef ENGINE_2_ITEMGOLD_H
#define ENGINE_2_ITEMGOLD_H
#include "../Item.h"

class ItemGold : public Item {
public:
    ItemGold() = default;
    explicit ItemGold(ivec2 tile);
    explicit ItemGold(vec2 pos);
    ~ItemGold() override = default;
    [[nodiscard]] TypeID getType() const override;

    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;
};


#endif //ENGINE_2_ITEMGOLD_H