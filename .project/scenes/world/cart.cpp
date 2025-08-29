//
// Created by cobble on 8/29/2025.
//

#include "cart.h"

#include "world.h"

void Cart::setBlock(ivec3 blockPos) {
    this->progress = 0.0f;
    setSpeed(0.0f);
    this->backwards = false;
    this->blockPos = blockPos;
    if(blockPos == ivec3(-1)) this->block = nullptr;
    else this->block = WorldScene::getBlock(blockPos);
}

void Cart::update(float dt) {
            if(block == nullptr) return;
            if(WorldScene::railPaths.find(block->data) == WorldScene::railPaths.end()) return;

            RailTypeData* currRailTypeData = &WorldScene::railPaths[block->data];
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
                ivec3 railAfter = currRailTypeData->railAfter;
                ivec3 nextRailBlockPos = blockPos + railAfter;
                Block* nextRail = WorldScene::getBlock(nextRailBlockPos);
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    nextRailBlockPos += ivec3(0, -1, 0);
                    nextRail = WorldScene::getBlock(nextRailBlockPos);
                }
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    nextRailBlockPos += ivec3(0, 2, 0);
                    nextRail = WorldScene::getBlock(nextRailBlockPos);
                }
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    block = nullptr;
                    blockPos = ivec3(-1);
                    backwards = false;
                    progress = 0.0f;
                    std::cout << "no rail > 1.0f" << std::endl;
                    setSpeed(0.0f);
                    return;
                }
                RailTypeData* nextRailTypeData = &WorldScene::railPaths[nextRail->data];

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
                ivec3 railBefore = currRailTypeData->railBefore;

                ivec3 nextRailBlockPos = blockPos + railBefore;
                Block* nextRail = WorldScene::getBlock(nextRailBlockPos);
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    nextRailBlockPos += ivec3(0, -1, 0);
                    nextRail = WorldScene::getBlock(nextRailBlockPos);
                }
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    nextRailBlockPos += ivec3(0, 2, 0);
                    nextRail = WorldScene::getBlock(nextRailBlockPos);
                }
                if(nextRail == nullptr || nextRail->type != RAIL) {
                    block = nullptr;
                    blockPos = ivec3(-1);
                    backwards = false;
                    progress = 0.0f;
                    setSpeed(0.0f);
                    return;
                }
                RailTypeData* nextRailType = &WorldScene::railPaths[nextRail->data];

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

void Cart::draw(float w, float h) const {
    if(block != nullptr && block->type == RAIL) {
        if(WorldScene::railPaths.find(block->data) == WorldScene::railPaths.end()) return;

        vec2 pos = vec2(blockPos.z * -50 + blockPos.x * 50, blockPos.z * -25 + blockPos.x * -25 + blockPos.y * 50) + WorldScene::screenOffset + WorldScene::tempScreenOffset;
        RailTypeData *railData = &WorldScene::railPaths[block->data];

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
        WorldScene::blocks[CART_OFFSET + block->data].draw(pos.x, pos.y, w, h);
    }
}
