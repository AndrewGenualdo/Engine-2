//
// Created by cobble on 8/20/2025.
//

#ifndef TRACK_H
#define TRACK_H
#include <vector>

#include "structure.h"
#include "cobb/misc/animation.h"


class Track : public Structure {

public:
    static constexpr int BASIC = 0;
    static constexpr int END_LEFT = 1;
    static constexpr int END_RIGHT = 2;
    static constexpr int BASIC_TO_DIAG_RIGHT = 3;
    static constexpr int DIAG_RIGHT = 4;

    static constexpr int TRACK_TYPE_COUNT = 5;


    explicit Track() = default;
    explicit Track(vec2 tile);
    ~Track() override;

    void load() override;
    void cleanup() override;
    void update(float dt) override;
    void draw() override;
    void setTexture(std::string texturePath) override;
    std::string name() const override;
    void interract(Item *item, float dt) override;

    int type = BASIC;
    bool reversed = false;
    float speed = 0;

    std::vector<std::pair<vec2, vec2>> paths = std::vector<std::pair<vec2, vec2>>();

    static Animation trackBasic;
    static Animation trackEndLeft;
    static Animation trackEndRight;
    static Animation trackBasicToDiagonalRight;
    static Animation trackDiagonalRight;

    void updatePath();
};



#endif //TRACK_H
