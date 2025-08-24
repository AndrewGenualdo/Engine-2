//
// Created by cobble on 8/12/2025.
//

#ifndef TEAM_H
#define TEAM_H
#include "littleGuy.h"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

class LittleGuy;

class Team {
public:
        int spawnerTile;
        int coins;
        vec3 color;
        LittleGuy *guys;
        int guyCount;
        int teamGoalTile;
        int index;
        std::vector<int> teamGoalTileList = std::vector<int>();
        std::vector<int> connectedStructures = std::vector<int>();
        std::vector<int> connectedPaths = std::vector<int>();


        Team(int index, int spawnerTile, vec3 color);
        ~Team();

        void load();
        void cleanup();

        void update(float deltaTime);
        void draw() const;

        static float getGuyCost(int guys);
        float getGuyCost() const;

        float getScore() const;
        void getConnectedTiles();
        static std::vector<int> getTilesAlongLine(vec2 start, vec2 end, int gridWidth);
        void getGoalTile();
        bool addGuy(int tile);




    };



#endif //TEAM_H
