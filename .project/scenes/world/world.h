//
// Created by cobble on 8/27/2025.
//

#ifndef WORLD_H
#define WORLD_H
#include <map>

#include "cobb/core/camera.h"
#include "cobb/core/scene.h"
#include "cobb/misc/fontRenderer.h"
#include "cobb/misc/texture2d.h"

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

#define DEFAULT 0
#define NORTH 1
#define EAST 2
#define SOUTH 6
#define WEST 24

#define DOWN 0
#define UP_FIRST 1
#define UP_SECOND 3


class WorldScene : public Scene {
public:
    static Camera camera;
    static Window *window;

    WorldScene() = default;
    explicit WorldScene(Window *w);
    virtual ~WorldScene();

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;

    static FontRenderer fontRenderer;
    static Texture2d blank;
    static bool debugMode;

    bool wasLeftMouseDown = false;
    bool wasMiddleMouseDown = false;
    bool wasRightMouseDown = false;
    vec2 mouseStart = vec2(0);
    vec2 screenOffset = vec2(500);
    vec3 lastBelt = vec3(-1);
    vec3 lastHoverBlock = vec3(-1);
    constexpr static int WORLD_SIZE = 10;
    static std::map<int, Texture2d> blocks;
    static std::map<int, int*> blockVariants;

    struct Block {
        int type = 0;
        int data = 0;

        explicit Block(int type = 0, int data = 0) {
            this->type = type;
            this->data = data;
        }

        void draw(float x, float y, float w, float h) const {
            //default = 0
            //north = 1
            //east = 2
            //south = 3
            //west = 4
            //EX: default -> west = 04, EX2: south -> east = 32
            if(type == BELT) blocks[BELT_OFFSET + data].draw(x, y, w, h);
            else if(type == RAIL) blocks[RAIL_OFFSET + data].draw(x, y, w, h);
            else blocks[type].draw(x, y, w, h);
        }

        void setBeltTo(int dir) {
            int from = data / 10;
            if(dir != from) data = from * 10 + dir;
        }

        void setBeltFrom(int dir) {
            int to = data % 10;
            if(dir != to) data = dir * 10 + to;
        }

        void setRailType(int type) {
            data = type;
        }

    };

    Block ***world = nullptr;
};



#endif //WORLD_H
