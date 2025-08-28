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
#include "ew/ewMath/ewMath.h"

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
    static vec2 screenOffset;
    static vec2 tempScreenOffset;
    vec3 lastBelt = vec3(-1);
    vec3 lastHoverBlock = vec3(-1);
    constexpr static int WORLD_SIZE = 10;
    static std::map<int, Texture2d> blocks;
    static std::map<int, int*> blockVariants;

    struct RailTypeData {
        std::vector<vec2> pathPoints;
        vec3 railBefore;
        vec3 railAfter;

        RailTypeData() = default;

        RailTypeData(vec3 railBefore, vec3 railAfter,std::initializer_list<vec2> pathPoints) {
            this->pathPoints = std::vector<vec2>();
            for(vec2 point : pathPoints) this->pathPoints.push_back(point);
            this->railBefore = railBefore;
            this->railAfter = railAfter;
        }
    };

    static std::map<int, RailTypeData> railPaths;

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

    static Block ***world;

    struct Cart {
        Block* block = nullptr;
        vec3 blockPos = vec3(0);
        bool backwards = false;
        float speed = 0.0f;
        float progress = 0.0f; //0.0 -> 1.0 based on how far along the current track it is

        void setBlock(vec3 blockPos) {
            this->progress = 0.0f;
            this->speed = 1.0f;
            this->backwards = false;
            this->blockPos = blockPos;
            if(blockPos == vec3(-1)) this->block = nullptr;
            else this->block = &world[static_cast<int>(blockPos.y)][static_cast<int>(blockPos.x)][static_cast<int>(blockPos.z)];
        }

        explicit Cart(vec3 blockPos = vec3(-1), bool backwards = false, float speed = 1.0f, float progress = 0.0f) {
            this->setBlock(blockPos);
            this->backwards = backwards;
            this->speed = speed;
            this->progress = progress;
        }

        void update(float dt) {
            if(block == nullptr) return;
            //
            if(railPaths.find(block->data) == railPaths.end()) return;
            progress += speed * dt * (backwards ? -1 : 1);
            /*if(progress > 1.0f) progress -= 1.0f;
            if(progress < 0.0f) progress += 1.0f;*/
            RailTypeData* currRailTypeData = &railPaths[block->data];
            if(progress > 1.0f) {
                float diff = progress - 1.0f;
                //vec3 railBefore = backwards ? currRailTypeData->railAfter : currRailTypeData->railBefore;
                //vec3 railAfter = backwards ? currRailTypeData->railBefore : currRailTypeData->railAfter;
                vec3 railAfter = currRailTypeData->railAfter;
                vec3 nextRailBlockPos = blockPos + railAfter;
                Block* nextRail = &world[static_cast<int>(nextRailBlockPos.y)][static_cast<int>(nextRailBlockPos.x)][static_cast<int>(nextRailBlockPos.z)];
                RailTypeData* nextRailTypeData = &railPaths[nextRail->data];

                bool startAtEnd = false;
                //how do I determine if it should startAtEnd?
                if(nextRailTypeData->railBefore == -railAfter) startAtEnd = false;
                else if(nextRailTypeData->railAfter == -railAfter) startAtEnd = true;
                else {
                    //no connection
                    speed = 0.0f;
                    progress = 0.5f;
                }

                backwards = startAtEnd;
                progress = startAtEnd ? 1.0f - diff : diff;
                blockPos = nextRailBlockPos;
                block = nextRail;
            } else if(progress < 0.0f) {
                float diff = -progress;
                //vec3 railBefore = backwards ? currRailTypeData->railAfter : currRailTypeData->railBefore;
                vec3 railBefore = currRailTypeData->railBefore;
                vec3 nextRailBlockPos = blockPos + railBefore;
                Block* nextRail = &world[static_cast<int>(nextRailBlockPos.y)][static_cast<int>(nextRailBlockPos.x)][static_cast<int>(nextRailBlockPos.z)];
                RailTypeData* nextRailType = &railPaths[nextRail->data];

                bool startAtEnd = false;
                if(nextRailType->railAfter == -railBefore) startAtEnd = true;
                else if(nextRailType->railBefore == -railBefore) startAtEnd = false;
                else {
                    //no connection
                    speed = 0.0f;
                    progress = 0.5f;
                }

                backwards = startAtEnd;
                progress = startAtEnd ? 1.0f - diff : diff;
                blockPos = nextRailBlockPos;
                block = nextRail;
            }
        }

        void draw(float w, float h) const {
            if(block != nullptr && block->type == RAIL) {
                if(railPaths.find(block->data) == railPaths.end()) return;

                vec2 pos = vec2(blockPos.z * -50 + blockPos.x * 50, blockPos.z * -25 + blockPos.x * -25 + blockPos.y * 50) + screenOffset + tempScreenOffset;
                RailTypeData *railData = &railPaths[block->data];

                float totalDist = 0.0f;
                for(int i = 0; i < railData->pathPoints.size() - 1; i++) totalDist += distance(railData->pathPoints[i], railData->pathPoints[i+1]);
                //std::cout << totalDist << std::endl;
                vec2 delta = vec2(-1);
                float progress = this->progress;
                for(int i = 0; i < railData->pathPoints.size() - 1; i++) {
                    float dist = distance(railData->pathPoints[i], railData->pathPoints[i+1]);
                    float percOfRail = dist / totalDist;

                    if(progress > percOfRail) progress -= percOfRail;
                    else {
                        float lerpValue = progress / percOfRail;
                        delta = ew::lerp(railData->pathPoints[i], railData->pathPoints[i+1], lerpValue);
                        break;
                    }
                }
                if(delta == vec2(-1)) delta = railData->pathPoints[railData->pathPoints.size()-1];

                delta *= (w + h) * 0.5f;


                pos += delta;
                blank.draw(pos.x, pos.y, w, h);
            }
        }
    };


    Cart cart = Cart();

};



#endif //WORLD_H
