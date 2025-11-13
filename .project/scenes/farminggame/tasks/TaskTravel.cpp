//
// Created by cobble on 11/4/2025.
//

#include "TaskTravel.h"

#include <algorithm>
#include <queue>
#include <unordered_map>

#include "../misc/LittleGuy.h"

bool TaskTravel::generatePath() {
    pathIndex = 0;
    path.clear();

    constexpr ivec2 directions[4] = {
        { 0, -1 },
        { 0,  1 },
        { -1, 0 },
        { 1,  0 }
    };

    auto key = [](const ivec2& v) {
        return (static_cast<long long>(v.x) << 32) | (v.y & 0xffffffff);
    };

    //priority queue for dijkstra (lowest cost is first in the queue)
    struct Node {
        ivec2 pos;
        float priority;
        bool operator>(const Node& other) const {
            return priority > other.priority;
        }
    };

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier;
    std::unordered_map<long long, ivec2> cameFrom;
    std::unordered_map<long long, float> costSoFar;

    frontier.push({start, 0.0f});
    cameFrom[key(start)] = {-9999, -9999};
    costSoFar[key(start)] = 0.0f;

    while (!frontier.empty()) {
        ivec2 current = frontier.top().pos;
        frontier.pop();

        if (current == goal)
            break;

        for (const ivec2& dir : directions) {
            ivec2 next = current + dir;
            if (next.x < 0 || next.y < 0 || next.x >= FarmingWorld::TILES_HORIZ || next.y >= FarmingWorld::TILES_VERT) continue;

            float currentCost = costSoFar[key(current)];


            //bool currIsPath = world->getTile(current.x, current.y).landType == FarmingWorld::FARMLAND;
            //bool nextIsPath = world->getTile(next.x, next.y).landType == FarmingWorld::FARMLAND;
            //float moveCost = (currIsPath && nextIsPath) ? 0.5f : 1.0f;
            float moveCost = 1.0f;

            float newCost = currentCost + moveCost;

            if (costSoFar.find(key(next)) == costSoFar.end() || newCost < costSoFar[key(next)]) {
                costSoFar[key(next)] = newCost;
                float priority = newCost;
                frontier.push({next, priority});
                cameFrom[key(next)] = current;
            }
        }
    }


    if (cameFrom.find(key(goal)) == cameFrom.end())
        return false; //no path found

    ivec2 current = goal;
    while (current.x != -9999) {
        path.push_back(current);
        current = cameFrom[key(current)];
    }
    std::reverse(path.begin(), path.end());

    //print path points
    //std::cout << "| "; for (auto & i : path) std::cout << i.x << ", " << i.y << " | "; std::cout << std::endl;


    if (path.size() > 1) pathIndex = 1; //skip first tile
    cost = costSoFar[key(goal)];
    return true;
}

TaskTravel::TaskTravel(LittleGuy *guy, ivec2 goal) : Task(guy){
    this->guy = guy;
    this->start = guy->tile;
    this->goal = goal;
    generatePath();
}

TaskTravel::TaskTravel(LittleGuy *guy, ivec2 start, ivec2 goal) {
    this->guy = guy;
    this->start = start;
    this->goal = goal;
    generatePath();
}

bool TaskTravel::update(float dt) {
    if (guy->getTile() == goal) return true;
    if (path.empty()) generatePath();
    if (path.empty()) { //pathfinding has failed
        std::cout << "pathfinding has failed." << std::endl;
    }
    float travelDist = guy->getSpeed() * dt;
    //if (world->getTile(path[pathIndex].x, path[pathIndex].y).landType == FarmingWorld::FARMLAND) travelDist *= 2;

    while (travelDist > 0) {
        if (pathIndex >= path.size()) return true; //done pathfinding :D
        vec2 travel = FarmingWorld::getTilePos(path[pathIndex].x, path[pathIndex].y) - guy->getPos();
        float distToNext = length(FarmingWorld::getTilePos(path[pathIndex].x, path[pathIndex].y) - guy->getPos());

        if (distToNext > travelDist) {
            if (!(travel.x == 0 && travel.y == 0)) {
                vec2 dir = normalize(travel) * travelDist;
                guy->move(dir);
            }
            travelDist = 0;
        } else {
            guy->setPos(FarmingWorld::getTilePos(path[pathIndex].x, path[pathIndex].y));
            guy->setTile(path[pathIndex]);
            travelDist -= distToNext;
            pathIndex++;
        }
    }


    return false;
}

float TaskTravel::getCost() {
    return FarmingWorld::TICKS_PER_SECOND * cost * guy->getSpeed() / FarmingWorld::TILE_WIDTH;
}

std::string TaskTravel::getName() {
    return "Travel: {"+std::to_string(goal.x) + ", " + std::to_string(goal.y) + "} ";
}
