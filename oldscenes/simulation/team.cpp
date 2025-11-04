//
// Created by cobble on 8/12/2025.
//

#include "team.h"

#include "simulation.h"
#include "ew/ewMath/ewMath.h"

Team::Team(int index, int spawnerTile, vec3 color)  {
    this->coins = 0;
    this->guys = nullptr;
    this->guyCount = 0;
    this->spawnerTile = spawnerTile;
    this->index = index;
    this->color = color;
    this->teamGoalTile = -1;
}

Team::~Team(){
    cleanup();
}

void Team::load() {
    cleanup();
    this->coins = 50;
    this->guys = new LittleGuy[SimulationScene::MAX_GUYS];
    for(int i=0;i< SimulationScene::MAX_GUYS;i++) guys[i] = LittleGuy();
    this->guyCount = 0;
    this->teamGoalTile = -1;
    this->teamGoalTileList.clear();
    this->connectedPaths.clear();
    this->connectedStructures.clear();
    SimulationScene::tiles[spawnerTile].type = SPAWNER;
    SimulationScene::tiles[spawnerTile].data = index;
    bool placedGenerator = false;
    for(int y = -1; y <= 1; y++) {
        for(int x = -1; x <= 1; x++) {
            int pos = spawnerTile + x + y * SimulationScene::GRID_WIDTH;
            if(pos != spawnerTile && SimulationScene::validTile(pos) && !SimulationScene::wraps(spawnerTile, pos)) {
                if(!placedGenerator && y == 0) {
                    SimulationScene::tiles[pos].type = GENERATOR;
                    placedGenerator = true;
                }
                else SimulationScene::tiles[pos].type = PATH;

            }
        }
    }
    getConnectedTiles();
}

void Team::cleanup()  {
    delete [] guys;
    guys = nullptr;
}

void Team::update(float deltaTime) {

    if(teamGoalTile == -1) getGoalTile();
    else {
        for(int i = 0; i < connectedPaths.size(); i++) {
            if(connectedPaths[i] == teamGoalTile) {
                getGoalTile();
                break;
            }
        }
        for(int i = 0; i < connectedStructures.size(); i++) {
            if(connectedStructures[i] == teamGoalTile) {
                getGoalTile();
                break;
            }
        }

    }

    for(int i = 0; i < SimulationScene::MAX_GUYS; i++) if(guys[i].exists) guys[i].update(deltaTime);
}

void Team::draw() const {
    for(int i = 0; i < SimulationScene::MAX_GUYS; i++) if(guys[i].exists) guys[i].draw();
}

float Team::getGuyCost(int guys) {
    if(guys == SimulationScene::MAX_GUYS) return -1.0f;
    int bracket = guys / (SimulationScene::MAX_GUYS / SimulationScene::GUY_COST_BRACKETS);
    return SimulationScene::GUY_COST[bracket];
}

float Team::getGuyCost() const {
    return getGuyCost(guyCount);
}

float Team::getScore() const {
    if(SimulationScene::tiles[spawnerTile].data != index && guyCount == 0) return 0.0f;
    float score = coins;
    int guyCount = 0;
    for(int i = 0; i < SimulationScene::MAX_GUYS; i++) {
        if(guys[i].exists) {
            int bracket = guyCount / (SimulationScene::MAX_GUYS / SimulationScene::GUY_COST_BRACKETS);
            score += SimulationScene::GUY_COST[bracket] * guys[i].health / guys[i].maxHealth;
            guyCount++;
        }
    }
    return score;
}

void Team::getConnectedTiles() {

    connectedPaths.clear();
    connectedStructures.clear();
    int index = 0;
    connectedPaths.push_back(spawnerTile);
    connectedStructures.push_back(spawnerTile);
    while(index < connectedPaths.size()) {
        int tileOffsets[] = {-1, -SimulationScene::GRID_WIDTH, 1, SimulationScene::GRID_WIDTH};
        for(int i = 0; i < 4; i++) {
            int tile = tileOffsets[i] + connectedPaths[index];
            if(SimulationScene::validTile(tile) && !SimulationScene::wraps(tile, connectedPaths[index])) {
                if(SimulationScene::tiles[tile].type == PATH || SimulationScene::tiles[tile].type == SPAWNER || SimulationScene::tiles[tile].type == BRIDGE) {
                    bool found = false;
                    for(int j = 0; j < connectedPaths.size(); j++) {
                        if(connectedPaths[j] == tile) {
                            found = true;
                            break;
                        }
                    }
                    if(!found) connectedPaths.push_back(tile);
                } else if(SimulationScene::tiles[tile].type == GENERATOR || SimulationScene::tiles[tile].type == BANK) {
                    bool found = false;
                    for(int j = 0; j < connectedStructures.size(); j++) {
                        if(connectedStructures[j] == tile) {
                            found = true;
                            break;
                        }
                    }
                    if(!found) connectedStructures.push_back(tile);
                }
            }
        }
        index++;
    }
}

std::vector<int> Team::getTilesAlongLine(glm::vec2 start, glm::vec2 end, int gridWidth) {
    std::vector<int> result;

    int x = (int)floor(start.x);
    int y = (int)floor(start.y);

    int endX = (int)floor(end.x);
    int endY = (int)floor(end.y);

    float dx = end.x - start.x;
    float dy = end.y - start.y;

    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    // Avoid divide-by-zero
    float tDeltaX = (dx != 0) ? std::abs(1.0f / dx) : INFINITY;
    float tDeltaY = (dy != 0) ? std::abs(1.0f / dy) : INFINITY;

    // Calculate distance to first vertical and horizontal boundary
    float nextVertGrid = (dx > 0) ? (float(x + 1) - start.x) : (start.x - float(x));
    float nextHorizGrid = (dy > 0) ? (float(y + 1) - start.y) : (start.y - float(y));

    float tMaxX = (dx != 0) ? nextVertGrid / std::abs(dx) : INFINITY;
    float tMaxY = (dy != 0) ? nextHorizGrid / std::abs(dy) : INFINITY;

    result.push_back(y * gridWidth + x);

    while (x != endX || y != endY) {
        if (tMaxX < tMaxY) {
            x += stepX;
            tMaxX += tDeltaX;
        }
        else if (tMaxY < tMaxX) {
            y += stepY;
            tMaxY += tDeltaY;
        }
        else {
            // Hitting a perfect corner → pick one consistently
            x += stepX;
            y += stepY;
            tMaxX += tDeltaX;
            tMaxY += tDeltaY;
        }
        result.push_back(y * gridWidth + x);
    }

    return result;
}

void Team::getGoalTile() {
            std::cout << "Getting goal tile for team with spawnerTile: " << spawnerTile << std::endl;
            teamGoalTileList.clear();
            std::vector<int> generators = std::vector<int>();
            for(int i = 0; i < SimulationScene::GRID_WIDTH * SimulationScene::GRID_HEIGHT; i++) {
                if(SimulationScene::tiles[i].type == GENERATOR || SimulationScene::tiles[i].type == BANK) {
                    bool skip = false;
                    for(int j = 0; j < connectedStructures.size(); j++) {
                        if(connectedStructures[j] == i) {
                            skip = true;
                            break;
                        }

                    }
                    if(!skip) generators.push_back(i);
                }

            }

            if(generators.empty()) {
                teamGoalTile = -2;
                return;
            }

            float closestDist = FLT_MAX;
            int closestPath = -1;
            int closestGen = -1;

            for(int i = 0; i < generators.size();i++) {
                for(int j = 0; j < connectedPaths.size(); j++) {
                    vec2 genPos = vec2(generators[i] % SimulationScene::GRID_WIDTH, generators[i] / SimulationScene::GRID_WIDTH);
                    vec2 pathPos = vec2(connectedPaths[j] % SimulationScene::GRID_WIDTH, connectedPaths[j] / SimulationScene::GRID_WIDTH);
                    float dist = distance(genPos, pathPos);
                    if(dist < closestDist) {
                        closestGen = generators[i];
                        closestPath = connectedPaths[j];
                        closestDist = dist;
                    }
                }
            }

            std::cout << spawnerTile << " is pathing: " << closestGen << std::endl;
            teamGoalTile = closestGen;
            teamGoalTileList.clear();
            vec2 fromPos = vec2(closestPath % SimulationScene::GRID_WIDTH, closestPath / SimulationScene::GRID_WIDTH) * SimulationScene::TILE_DIMS + SimulationScene::TILE_DIMS * 0.5f;
            vec2 goalPos = vec2(closestGen % SimulationScene::GRID_WIDTH, closestGen / SimulationScene::GRID_WIDTH) * SimulationScene::TILE_DIMS + SimulationScene::TILE_DIMS * 0.5f;
            bool goForBridge = false;

            for(int i = 0; i < SimulationScene::GRID_WIDTH * SimulationScene::GRID_HEIGHT; i++) {
                vec2 pos = vec2(i % SimulationScene::GRID_WIDTH, i / SimulationScene::GRID_WIDTH) * SimulationScene::TILE_DIMS;
                vec2 closestPoint = ew::closestPointOnLine(fromPos, goalPos, pos);
                if(distance(closestPoint, pos) < 0.75f * SimulationScene::TILE_DIMS.x) {
                    if(SimulationScene::tiles[i].type == WATER) {
                        goForBridge = true;
                        break;
                    }
                    if(SimulationScene::tiles[i].type == GRASS || SimulationScene::tiles[i].type == TREE || SimulationScene::tiles[i].type == BRIDGE) {
                        teamGoalTileList.push_back(i);
                    } else if(SimulationScene::tiles[i].type == GENERATOR || SimulationScene::tiles[i].type == BANK) {
                        bool skip = false;
                        //for(int j = 0; j < connectedStructures.size(); j++) if(i == connectedStructures[j]) skip = true;

                        if(!skip) {
                            for(int y = -1; y <= 1; y++) {
                                for(int x = -1; x <= 1; x++) {
                                    if(x == 0 && y == 0) continue;
                                    vec2 tile = vec2(i % SimulationScene::GRID_WIDTH + x, i / SimulationScene::GRID_WIDTH + y);
                                    int tileIndex = tile.y * SimulationScene::GRID_WIDTH + tile.x;
                                    if(!SimulationScene::validTile(tileIndex) || SimulationScene::wraps(i, tileIndex)) continue;
                                    int type = SimulationScene::tiles[tileIndex].type;
                                    if(type == GRASS || type == TREE) {
                                        teamGoalTileList.push_back(tileIndex);
                                    }

                                }
                            }
                        }
                    }
                }
            }

            if(goForBridge) {
                teamGoalTileList.clear();
                std::vector<int> bridges = std::vector<int>();
                for(int i = 0; i < SimulationScene::GRID_WIDTH * SimulationScene::GRID_HEIGHT; i++) {
                    if(SimulationScene::tiles[i].type == BRIDGE) bridges.push_back(i);
                }

                float bridgeDist = FLT_MAX;
                int fromPath = -1;
                int toBridge = -1;
                for(int i = 0; i < connectedPaths.size(); i++) {
                    for(int j = 0; j < bridges.size(); j++) {
                        vec2 bridgePos = vec2(bridges[j] % SimulationScene::GRID_WIDTH, bridges[j] / SimulationScene::GRID_WIDTH);
                        vec2 pathPos = vec2(connectedPaths[i] % SimulationScene::GRID_WIDTH, connectedPaths[i] / SimulationScene::GRID_WIDTH);
                        float dist = distance(bridgePos, pathPos);
                        if(dist < bridgeDist) {
                            fromPath = connectedPaths[i];
                            toBridge = bridges[j];
                            bridgeDist = dist;
                        }
                    }
                }
                teamGoalTile = toBridge;
                fromPos = vec2(fromPath % SimulationScene::GRID_WIDTH, fromPath / SimulationScene::GRID_WIDTH) * SimulationScene::TILE_DIMS + SimulationScene::TILE_DIMS * 0.5f;
                goalPos = vec2(toBridge % SimulationScene::GRID_WIDTH, toBridge / SimulationScene::GRID_WIDTH) * SimulationScene::TILE_DIMS + SimulationScene::TILE_DIMS * 0.5f;

                for(int i = 0; i < SimulationScene::GRID_WIDTH * SimulationScene::GRID_HEIGHT; i++) {
                    vec2 pos = vec2(i % SimulationScene::GRID_WIDTH, i / SimulationScene::GRID_WIDTH) * SimulationScene::TILE_DIMS;
                    vec2 closestPoint = ew::closestPointOnLine(fromPos, goalPos, pos);
                    if(distance(closestPoint, pos) < 0.75f * SimulationScene::TILE_DIMS.x) {
                        if(SimulationScene::tiles[i].type == GRASS || SimulationScene::tiles[i].type == TREE || SimulationScene::tiles[i].type == WATER) {
                            teamGoalTileList.push_back(i);
                        } else if(SimulationScene::tiles[i].type == GENERATOR || SimulationScene::tiles[i].type == BANK) {
                            for(int y = -1; y <= 1; y++) {
                                for(int x = -1; x <= 1; x++) {
                                    if(x == 0 && y == 0) continue;
                                    vec2 tile = vec2(i % SimulationScene::GRID_WIDTH + x, i / SimulationScene::GRID_WIDTH + y);
                                    int tileIndex = tile.y * SimulationScene::GRID_WIDTH + tile.x;
                                    if(SimulationScene::validTile(tileIndex) && !SimulationScene::wraps(i, tileIndex) && (SimulationScene::tiles[tileIndex].type == GRASS || SimulationScene::tiles[tileIndex].type == TREE)) {
                                        teamGoalTileList.push_back(tileIndex);
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }

bool Team::addGuy(int tile) {
    if(guyCount >= SimulationScene::MAX_GUYS) return false;

    for(int i = 0; i < SimulationScene::MAX_GUYS; i++) {
        if(!guys[i].exists) {
            guys[i] = LittleGuy::createGuy(tile);
            guys[i].teamIndex = index;
            guyCount++;
            return true;
        }
    }

    return false;
}


