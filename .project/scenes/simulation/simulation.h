//
// Created by cobble on 6/10/2025.
//

#ifndef ENGINE_2_SIMULATION_H
#define ENGINE_2_SIMULATION_H

#define GRASS 0
#define PATH 1
#define GENERATOR 2
#define BANK 3
#define WATER 4
#define BRIDGE 5
#define TREE 6
#define SAND 7
#define SPAWNER 8
#define BASIC_TILE_COUNT 2
#define EVERY_TILE_COUNT 9

#define NONE 0
#define GOLD 1


#include "cobb/core/scene.h"
#include "cobb/core/camera.h"
#include "cobb/misc/fontRenderer.h"
#include <glm/gtx/rotate_vector.hpp>

#include "team.h"
#include "cobb/shapes/circle2d.h"

class LittleGuy;
class Team;

class SimulationScene : public Scene {
    struct Tile {
        int type;
        int data;
        float height;
    };

public:
    static Camera camera;
    static Window *window;

    explicit SimulationScene() = default;
    explicit SimulationScene(Window *w);
    virtual ~SimulationScene() override;

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;



    static FontRenderer fontRenderer;
    static Texture2d blank;
    bool wasMouseDown = false;
    bool wasRightMouseDown = false;
    bool debugMode = false;

    constexpr static float MIN_SPEED = 0.5f;
    constexpr static float MAX_SPEED = 10.0f;
    constexpr static float MIN_SCALE = 0.5f;
    constexpr static float MAX_SCALE = 3.0f;
    constexpr static float SPEED_MIN_HEIGHT = 10.0f;
    constexpr static float SPEED_SCALE_MULT = 2.0f;
    constexpr static float SPEED_HEIGHT_MULT = 2.0f;
    constexpr static float ITEM_SCALE = 0.5f;
    constexpr static int GENERATOR_COUNT = 18;
    constexpr static int BANK_COUNT = 10;
    constexpr static float MIN_BANK_DISTANCE = 100;//250.0f;

    static Tile *tiles;
    static float peak;
    constexpr static vec2 TILE_DIMS = vec2(40, 40);
    constexpr static int GRID_WIDTH = 25;//48;
    constexpr static int GRID_HEIGHT = 25;
    constexpr static int PATH_COST = 1;
    constexpr static int TREE_COST = 3;
    constexpr static int WATER_COST = 5;
    constexpr static int MAX_GUYS = 50;
    constexpr static vec2 GUY_DIMS = vec2(15, 15);
    constexpr static int GUY_COST_BRACKETS = 5;
    constexpr static int GUY_COST[5] = {5, 10, 15, 20, 25};
    constexpr static float TREE_CHANCE = 0.05;
    constexpr static float HEIGHT_STRENGTH = 0.35f;
    constexpr static float GUY_MAX_HEALTH = 100.0f;
    constexpr static float GUY_HEALING = 0.02f;
    constexpr static float GUY_MAX_HEALTH_REDUCTION = 0.0075f;
private:
    static Team green;
    static Team red;
    static Team blue;
    static Team purple;
public:
    constexpr static int TEAM_COUNT = 4;
    static Team *teams[TEAM_COUNT];




    static void drawBackground(bool pride);
    static bool isPath(int tile);
    static bool wraps(int tile1, int tile2);
    static bool validTile(int tile);
    static void generateRiver(float riverWidth, float bridgeWidth);
    static float getSurroundingHeights(int tile);
    static bool isNextToWater(int tile);
    static void generateHeights(float heightVaryMin, float heightVaryMax, float minHeight, float maxHeight);

};


#endif //ENGINE_2_SIMULATION_H
