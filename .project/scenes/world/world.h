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
        constexpr static float FRICTION = 0.1f;
        constexpr static float GRAVITY = 2.5f;
        std::vector<vec2> pathPoints;
        vec3 railBefore;
        vec3 railAfter;
        float gravity;

        RailTypeData() = default;

        RailTypeData(vec3 railBefore, vec3 railAfter,std::initializer_list<vec2> pathPoints) {
            this->pathPoints = std::vector<vec2>();
            for(vec2 point : pathPoints) this->pathPoints.push_back(point);
            this->railBefore = railBefore;
            this->railAfter = railAfter;
            this->gravity = abs(railBefore.y - railAfter.y) * GRAVITY;
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

    static Block* getBlock(vec3 pos) {
        if(pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE || pos.z >= WORLD_SIZE || pos.x < 0 || pos.y < 0 || pos.z < 0) return nullptr;
        return &world[static_cast<int>(pos.y)][static_cast<int>(pos.x)][static_cast<int>(pos.z)];
    }

    struct Cart {
        Block* block = nullptr;
        vec3 blockPos = vec3(0);
        bool backwards = false;
        float speed = 0.0f;
        float progress = 0.0f; //0.0 -> 1.0 based on how far along the current track it is

        void setSpeed(float speed) {
            this->speed = speed;
        }

        void setBlock(vec3 blockPos) {
            this->progress = 0.0f;
            setSpeed(0.0f);
            this->backwards = false;
            this->blockPos = blockPos;
            if(blockPos == vec3(-1)) this->block = nullptr;
            else this->block = getBlock(blockPos);
        }

        explicit Cart(vec3 blockPos = vec3(-1), bool backwards = false, float speed = 0.0f, float progress = 0.0f) {
            this->setBlock(blockPos);
            this->backwards = backwards;
            setSpeed(speed);
            this->progress = progress;
        }

        void update(float dt) {
            if(block == nullptr) return;
            if(railPaths.find(block->data) == railPaths.end()) return;

            RailTypeData* currRailTypeData = &railPaths[block->data];
            speed += currRailTypeData->gravity * dt * (backwards ? -1 : 1);
            if(speed < 0) {
                backwards = !backwards;
                speed = -speed;
            }

            if(abs(speed) < RailTypeData::FRICTION * dt) setSpeed(0);
            else if(speed > RailTypeData::FRICTION * dt) setSpeed(speed - RailTypeData::FRICTION * dt);
            else if(speed < -RailTypeData::FRICTION * dt) setSpeed(speed + RailTypeData::FRICTION * dt);
            progress += speed * dt * (backwards ? -1 : 1);



            if(progress > 1.0f) {
                float diff = progress - 1.0f;
                vec3 railAfter = currRailTypeData->railAfter;
                vec3 nextRailBlockPos = blockPos + railAfter;
                Block* nextRail = getBlock(nextRailBlockPos);
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    nextRailBlockPos += vec3(0.0f, -1.0f, 0.0f);
                    nextRail = getBlock(nextRailBlockPos);
                }
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    nextRailBlockPos += vec3(0.0f, 2.0f, 0.0f);
                    nextRail = getBlock(nextRailBlockPos);
                }
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    block = nullptr;
                    blockPos = vec3(-1.0f);
                    backwards = false;
                    progress = 0.0f;
                    std::cout << "no rail > 1.0f" << std::endl;
                    setSpeed(0.0f);
                    return;
                }
                RailTypeData* nextRailTypeData = &railPaths[nextRail->data];

                bool startAtEnd = false;
                //how do I determine if it should startAtEnd?
                if(nextRailTypeData->railBefore == -railAfter) startAtEnd = false;
                else if(nextRailTypeData->railAfter == -railAfter) startAtEnd = true;
                else {
                    //no connection
                }

                backwards = startAtEnd;
                progress = startAtEnd ? 1.0f - diff : diff;
                blockPos = nextRailBlockPos;
                block = nextRail;
            } else if(progress < 0.0f) {
                float diff = -progress;
                vec3 railBefore = currRailTypeData->railBefore;

                vec3 nextRailBlockPos = blockPos + railBefore;
                Block* nextRail = getBlock(nextRailBlockPos);
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    nextRailBlockPos += vec3(0.0f, -1.0f, 0.0f);
                    nextRail = getBlock(nextRailBlockPos);
                }
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    nextRailBlockPos += vec3(0.0f, 2.0f, 0.0f);
                    nextRail = getBlock(nextRailBlockPos);
                }
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    block = nullptr;
                    blockPos = vec3(-1.0f);
                    backwards = false;
                    progress = 0.0f;
                    setSpeed(0.0f);
                    return;
                }
                RailTypeData* nextRailType = &railPaths[nextRail->data];

                bool startAtEnd = false;
                if(nextRailType->railAfter == -railBefore) startAtEnd = true;
                else if(nextRailType->railBefore == -railBefore) startAtEnd = false;
                else {
                    //no connection
                }

                //I have no idea why this is needed but it needs it to work
                if(backwards && block->data > 1 && block->data % 2 == 1 && nextRail->data > 1 && nextRail->data % 2 == 1 && blockPos.y != nextRailBlockPos.y) startAtEnd = true;

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
