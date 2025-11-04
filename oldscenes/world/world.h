//
// Created by cobble on 8/27/2025.
//

#ifndef WORLD_H
#define WORLD_H
#include <map>

#include "block.h"
#include "cart.h"
#include "railTypeData.h"
#include "cobb/core/camera.h"
#include "cobb/core/scene.h"
#include "cobb/misc/fontRenderer.h"
#include "cobb/misc/texture2d.h"
#include "ew/ewMath/ewMath.h"
#include <iomanip>
#include <sstream>

#define TEMPLATE -1
#define AIR 0
#define GRASS 1
#define DIRT 2
#define STONE 3
#define LOG 4
#define BELT 5
#define RAIL 6

#define BLOCK_TYPES 7

#define BELT_OFFSET 1000
#define RAIL_OFFSET 2000
#define CART_OFFSET 3000

#define DEFAULT 0
#define NORTH 1
#define EAST 2
#define SOUTH 6
#define WEST 24

#define DOWN 120
#define UP 720

#define UP_FIRST 1
#define UP_SECOND 3


class WorldScene final : public Scene {
public:
    static Camera camera;
    static Window *window;

    WorldScene() = default;
    explicit WorldScene(Window *w);
    ~WorldScene() override;

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;
    void scrollWheel(float xOff, float yOff) override;

    static FontRenderer fontRenderer;
    static Texture2d blank;
    static bool debugMode;

    vec2 mouseStart = vec2(0);
    static vec2 screenOffset;
    static vec2 tempScreenOffset;
    constexpr static int WORLD_SIZE = 10;
    static std::map<int, Texture2d> blocks;
    static std::map<int, int*> blockVariants;
    static std::map<int, RailTypeData> railPaths;
    int zoom = 0;

    static Block ***world;

    static Block* getBlock(ivec3 pos) {
        if(pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE || pos.z >= WORLD_SIZE || pos.x < 0 || pos.y < 0 || pos.z < 0) return nullptr;
        return &world[pos.y][pos.x][pos.z];
    }

    static void setBlock(ivec3 pos, Block block) {
        if(pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE || pos.z >= WORLD_SIZE || pos.x < 0 || pos.y < 0 || pos.z < 0) return;
        world[pos.y][pos.x][pos.z] = block;
    }

    static void setPreviewBlock(ivec3 pos, Block block) {
        if(pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE || pos.z >= WORLD_SIZE || pos.x < 0 || pos.y < 0 || pos.z < 0) return;
        previewBlocks[pos] = block;
    }

    Cart cart = Cart();
    ivec3 railStart = ivec3(-1);
    ivec3 hoverBlock = ivec3(-1);


    struct IVec3Compare {
        bool operator()(const ivec3& a, const ivec3& b) const {
            return a.x + a.y * WORLD_SIZE + a.z * WORLD_SIZE * WORLD_SIZE < b.x + b.y * WORLD_SIZE + b.z * WORLD_SIZE * WORLD_SIZE;
        }
    };
    static std::map<ivec3, Block, IVec3Compare> previewBlocks;



};



#endif //WORLD_H
