//
// Created by cobble on 8/29/2025.
//

#include "railTypeData.h"

#include "world.h"


RailTypeData::RailTypeData(ivec3 railBefore, ivec3 railAfter, std::initializer_list<vec2> pathPoints)  {
    this->pathPoints = std::vector<vec2>();
    for(vec2 point : pathPoints) this->pathPoints.push_back(point);
    this->railBefore = railBefore;
    this->railAfter = railAfter;
    this->gravity = abs(railBefore.y - railAfter.y) * GRAVITY;
}

void RailTypeData::placeRails(ivec3 railStart, ivec3 railEnd) {

    for (auto const& [key, val] : WorldScene::previewBlocks)WorldScene::setBlock(key, val);
    WorldScene::previewBlocks.clear();

    /*ivec3 dir = vec3(ew::normalize(railEnd.x - railStart.x), ew::normalize(railEnd.y - railStart.y), ew::normalize(railEnd.z - railStart.z));

    ivec3 pos = railStart;
    ivec3 amt = railEnd - railStart;
    int yEvery = amt.y == 0 ? INT_MAX : (abs(amt.x) + abs(amt.z)) / abs(amt.y);
    if(yEvery == 0) amt = ivec3(0);
    int totalRails = abs(amt.x) + abs(amt.z);
    int delayedY = 0;
    int railIndex = 0;
    ivec3 firstRail = ivec3(-1);
    ivec3 lastRail = ivec3(-1);
#define yCondition yEvery != INT_MAX && abs(amt.y) > 0

    while(abs(amt.x) > 0) {
        int railType = NORTH * SOUTH;
        bool isTurn = railStart + amt != railEnd && railEnd.x - railStart.x == amt.x && abs(railEnd.z - railStart.z) > 0;
        if(isTurn) railType = (dir.x > 0 ? SOUTH : NORTH) * (dir.z > 0 ? EAST : WEST);
        bool blockY = isTurn;
        if(delayedY > 0 && yCondition) {
            if(blockY) delayedY++;
            else {
                amt -= ivec3(0, dir.y, 0);
                delayedY--;
            }
        }
        else if(yCondition && railIndex % yEvery == yEvery / 2) {
            if(blockY) delayedY++;
            else {
                amt -= ivec3(0, dir.y, 0);
                railType += dir.x * -dir.y == 1 ? UP_FIRST : UP_SECOND;
            }
        }
        if(railType > 1 && railType % 2 == 1) {
            if(railEnd.y > railStart.y) {
                WorldScene::setBlock(pos + amt + ivec3(0, 1, 0), Block(AIR));
                WorldScene::setBlock(pos + amt,  Block(RAIL, railType));
            } else {
                WorldScene::setBlock(pos + amt, Block(AIR));
                WorldScene::setBlock(pos + amt - ivec3(0, 1, 0), Block(RAIL, railType));
            }

        } else WorldScene::setBlock(pos + amt, Block(RAIL, railType));
        if(railIndex == totalRails - 1) firstRail = pos + amt;
        if(railIndex == 1) lastRail = pos + amt;
        amt -= ivec3(dir.x, 0, 0);
        railIndex++;
    }

    while(abs(amt.z) > 0) {
        int railType = EAST * WEST;
        bool isTurn = railStart + amt != railEnd && railEnd.z - railStart.z == amt.z && abs(railEnd.x - railStart.x) > 0;
        if(isTurn) railType = (dir.x > 0 ? SOUTH : NORTH) * (dir.z > 0 ? EAST : WEST);
        bool blockY = isTurn;
        if(delayedY > 0 && yCondition) {
            if(blockY) delayedY++;
            else {
                amt -= ivec3(0, dir.y, 0);
                delayedY--;
            }
        }
        else if(yCondition && railIndex % yEvery == yEvery / 2) {
            if(blockY) delayedY++;
            else {
                amt -= ivec3(0, dir.y, 0);
                railType += dir.z * -dir.y == 1 ? UP_FIRST : UP_SECOND;
            }
        }

        if(railType > 1 && railType % 2 == 1) {
            if(railEnd.y > railStart.y) {
                WorldScene::setBlock(pos + amt + ivec3(0, 1, 0), Block(AIR));
                WorldScene::setBlock(pos + amt,  Block(RAIL, railType));
            } else {
                WorldScene::setBlock(pos + amt, Block(AIR));
                WorldScene::setBlock(pos + amt - ivec3(0, 1, 0), Block(RAIL, railType));
            }
        } else WorldScene::setBlock(pos + amt, Block(RAIL, railType));
        if(railIndex == totalRails - 1) firstRail = pos + amt;
        if(railIndex == 1) lastRail = pos + amt;
        amt -= ivec3(0, 0, dir.z);
        railIndex++;
    }

    if(firstRail != ivec3(-1)) {
        if(railStart.x > firstRail.x) WorldScene::setBlock(railStart, Block(RAIL, NORTH * NORTH));
        else if(railStart.x < firstRail.x) WorldScene::setBlock(railStart, Block(RAIL, SOUTH * SOUTH));
        else if(railStart.z > firstRail.z) WorldScene::setBlock(railStart, Block(RAIL, EAST * EAST));
        else if(railStart.z < firstRail.z) WorldScene::setBlock(railStart, Block(RAIL, WEST * WEST));
        else std::cout << "PANIC! THE RAILS ARE TELEPORTING" << std::endl;
    }
    if(lastRail != ivec3(-1) && !(WorldScene::getBlock(lastRail)->data > 1 && WorldScene::getBlock(lastRail)->data % 2 == 1)) {
        if(railEnd.x > lastRail.x) WorldScene::setBlock(railEnd, Block(RAIL, NORTH * NORTH));
        else if(railEnd.x < lastRail.x) WorldScene::setBlock(railEnd, Block(RAIL, SOUTH * SOUTH));
        else if(railEnd.z > lastRail.z) WorldScene::setBlock(railEnd, Block(RAIL, EAST * EAST));
        else if(railEnd.z < lastRail.z) WorldScene::setBlock(railEnd, Block(RAIL, WEST * WEST));
        else std::cout << "PANIC! THE RAILS ARE TELEPORTING" << std::endl;
    }*/
}

void RailTypeData::getRailPlacements(ivec3 railStart, ivec3 railEnd) {

    if(railStart == railEnd) {
        WorldScene::setPreviewBlock(railStart, Block(RAIL, NORTH * SOUTH));
        return;
    }

    ivec3 dir = vec3(ew::normalize(railEnd.x - railStart.x), ew::normalize(railEnd.y - railStart.y), ew::normalize(railEnd.z - railStart.z));

    ivec3 pos = railStart;
    ivec3 amt = railEnd - railStart;
    int yEvery = amt.y == 0 ? INT_MAX : (abs(amt.x) + abs(amt.z)) / abs(amt.y);
    if(yEvery == 0) amt = ivec3(0);
    int totalRails = abs(amt.x) + abs(amt.z);
    int delayedY = 0;
    int railIndex = 0;
    ivec3 firstRail = ivec3(-1);
    ivec3 lastRail = ivec3(-1);
#define yCondition yEvery != INT_MAX && abs(amt.y) > 0

    while(abs(amt.x) > 0) {
        int railType = NORTH * SOUTH;
        bool isTurn = railStart + amt != railEnd && railEnd.x - railStart.x == amt.x && abs(railEnd.z - railStart.z) > 0;
        if(isTurn) railType = (dir.x > 0 ? SOUTH : NORTH) * (dir.z > 0 ? EAST : WEST);
        bool blockY = isTurn;
        if(delayedY > 0 && yCondition) {
            if(blockY) delayedY++;
            else {
                amt -= ivec3(0, dir.y, 0);
                delayedY--;
            }
        }
        else if(yCondition && railIndex % yEvery == yEvery / 2) {
            if(blockY) delayedY++;
            else {
                amt -= ivec3(0, dir.y, 0);
                railType += dir.x * -dir.y == 1 ? UP_FIRST : UP_SECOND;
            }
        }
        if(railType > 1 && railType % 2 == 1) {
            if(railEnd.y > railStart.y) {
                WorldScene::setPreviewBlock(pos + amt + ivec3(0, 1, 0), Block(AIR));
                WorldScene::setPreviewBlock(pos + amt,  Block(RAIL, railType));
            } else {
                WorldScene::setPreviewBlock(pos + amt, Block(AIR));
                WorldScene::setPreviewBlock(pos + amt - ivec3(0, 1, 0), Block(RAIL, railType));
            }

        } else WorldScene::setPreviewBlock(pos + amt, Block(RAIL, railType));
        if(railIndex == totalRails - 1) firstRail = pos + amt;
        if(railIndex == 1) lastRail = pos + amt;
        amt -= ivec3(dir.x, 0, 0);
        railIndex++;
    }

    while(abs(amt.z) > 0) {
        int railType = EAST * WEST;
        bool isTurn = railStart + amt != railEnd && railEnd.z - railStart.z == amt.z && abs(railEnd.x - railStart.x) > 0;
        if(isTurn) railType = (dir.x > 0 ? SOUTH : NORTH) * (dir.z > 0 ? EAST : WEST);
        bool blockY = isTurn;
        if(delayedY > 0 && yCondition) {
            if(blockY) delayedY++;
            else {
                amt -= ivec3(0, dir.y, 0);
                delayedY--;
            }
        }
        else if(yCondition && railIndex % yEvery == yEvery / 2) {
            if(blockY) delayedY++;
            else {
                amt -= ivec3(0, dir.y, 0);
                railType += dir.z * -dir.y == 1 ? UP_FIRST : UP_SECOND;
            }
        }

        if(railType > 1 && railType % 2 == 1) {
            if(railEnd.y > railStart.y) {
                WorldScene::setPreviewBlock(pos + amt + ivec3(0, 1, 0), Block(AIR));
                WorldScene::setPreviewBlock(pos + amt,  Block(RAIL, railType));
            } else {
                WorldScene::setPreviewBlock(pos + amt, Block(AIR));
                WorldScene::setPreviewBlock(pos + amt - ivec3(0, 1, 0), Block(RAIL, railType));
            }
        } else WorldScene::setPreviewBlock(pos + amt, Block(RAIL, railType));
        if(railIndex == totalRails - 1) firstRail = pos + amt;
        if(railIndex == 1) lastRail = pos + amt;
        amt -= ivec3(0, 0, dir.z);
        railIndex++;
    }

    if(firstRail != ivec3(-1)) {
        if(railStart.x > firstRail.x) WorldScene::setPreviewBlock(railStart, Block(RAIL, NORTH * NORTH));
        else if(railStart.x < firstRail.x) WorldScene::setPreviewBlock(railStart, Block(RAIL, SOUTH * SOUTH));
        else if(railStart.z > firstRail.z) WorldScene::setPreviewBlock(railStart, Block(RAIL, EAST * EAST));
        else if(railStart.z < firstRail.z) WorldScene::setPreviewBlock(railStart, Block(RAIL, WEST * WEST));
        else std::cout << "PANIC! THE RAILS ARE TELEPORTING" << std::endl;
    }
    if(lastRail != ivec3(-1) && WorldScene::getBlock(lastRail) != nullptr && !(WorldScene::getBlock(lastRail)->data > 1 && WorldScene::getBlock(lastRail)->data % 2 == 1)) {
        if(railEnd.x > lastRail.x) WorldScene::setPreviewBlock(railEnd, Block(RAIL, NORTH * NORTH));
        else if(railEnd.x < lastRail.x) WorldScene::setPreviewBlock(railEnd, Block(RAIL, SOUTH * SOUTH));
        else if(railEnd.z > lastRail.z) WorldScene::setPreviewBlock(railEnd, Block(RAIL, EAST * EAST));
        else if(railEnd.z < lastRail.z) WorldScene::setPreviewBlock(railEnd, Block(RAIL, WEST * WEST));
        else std::cout << "PANIC! THE RAILS ARE TELEPORTING" << std::endl;
    }
}
