//
// Created by cobble on 8/12/2025.
//

#include "littleGuy.h"
LittleGuy::LittleGuy() {
    x = 0;
    y = 0;
    currentTile = 0;
    nextTile = 0;
    speed = 5.25;//ew::RandomRange(MIN_SPEED, MAX_SPEED);
    scale = 1.75;//ew::RandomRange(MIN_SCALE, MAX_SCALE);
    item = 0;
    exists = false;
    //team = ew::RandomRange(0, 1) > 0.5f ? &SimulationScene::green : &SimulationScene::red;
    teamIndex = 0;
    health = 50;
    maxHealth = 100;
}

void LittleGuy::action() {
    int goalTile = nextTile;
    int guyTile = currentTile;
    int diff = goalTile - guyTile;

    for(int i = 0; i < SimulationScene::TEAM_COUNT; i++) {
        if(SimulationScene::teams[i]->index == teamIndex) continue; //skip own team

        for(int j = 0; j < SimulationScene::MAX_GUYS; j++) {
            LittleGuy *guy = &SimulationScene::teams[i]->guys[j];
            if(!guy->exists) continue;
            if(distance(vec2(x, y), vec2(guy->x, guy->y)) < length(SimulationScene::TILE_DIMS)) {
                guy->takeDamage(health * 0.25f);
                diff = -diff;
                break;
            }
        }
    }

    currentTile = goalTile;
    if(diff == 0) { //no target
        if(SimulationScene::isPath(goalTile + SimulationScene::GRID_WIDTH)) goalTile = guyTile + SimulationScene::GRID_WIDTH;
        else if(SimulationScene::isPath(goalTile + 1) && !SimulationScene::wraps(goalTile, goalTile + 1)) goalTile = guyTile + 1;
        else if(SimulationScene::isPath(goalTile - SimulationScene::GRID_WIDTH)) goalTile = guyTile - SimulationScene::GRID_WIDTH;
        else if(SimulationScene::isPath(goalTile - 1) && !SimulationScene::wraps(goalTile, goalTile - 1)) goalTile = guyTile - 1;
        else goalTile = nextTile;
    } else if(diff == 1) { //right
        if(SimulationScene::isPath(goalTile + SimulationScene::GRID_WIDTH)) goalTile += SimulationScene::GRID_WIDTH;
        else if(SimulationScene::isPath(goalTile + 1) && !SimulationScene::wraps(goalTile, goalTile + 1)) goalTile += 1;
        else if(SimulationScene::isPath(goalTile - SimulationScene::GRID_WIDTH)) goalTile -= SimulationScene::GRID_WIDTH;
        else if(SimulationScene::isPath(goalTile - 1) && !SimulationScene::wraps(goalTile, goalTile - 1)) goalTile -= 1;
        else goalTile = nextTile;
    } else if(diff == -1) { //left
        if(SimulationScene::isPath(goalTile - SimulationScene::GRID_WIDTH)) goalTile -= SimulationScene::GRID_WIDTH;
        else if(SimulationScene::isPath(goalTile - 1) && !SimulationScene::wraps(goalTile, goalTile - 1)) goalTile -= 1;
        else if(SimulationScene::isPath(goalTile + SimulationScene::GRID_WIDTH)) goalTile += SimulationScene::GRID_WIDTH;
        else if(SimulationScene::isPath(goalTile + 1) && !SimulationScene::wraps(goalTile, goalTile + 1)) goalTile += 1;
        else goalTile = nextTile;
    } else if(diff == SimulationScene::GRID_WIDTH) { //up
        if(SimulationScene::isPath(goalTile - 1) && !SimulationScene::wraps(goalTile, goalTile - 1)) goalTile -= 1;
        else if(SimulationScene::isPath(goalTile + SimulationScene::GRID_WIDTH)) goalTile += SimulationScene::GRID_WIDTH;
        else if(SimulationScene::isPath(goalTile + 1) && !SimulationScene::wraps(goalTile, goalTile + 1)) goalTile += 1;
        else if(SimulationScene::isPath(goalTile - SimulationScene::GRID_WIDTH)) goalTile -= SimulationScene::GRID_WIDTH;
        else goalTile = nextTile;
    } else if(diff == -SimulationScene::GRID_WIDTH) { //down
        if(SimulationScene::isPath(goalTile + 1) && !SimulationScene::wraps(goalTile, goalTile + 1)) goalTile += 1;
        else if(SimulationScene::isPath(goalTile - SimulationScene::GRID_WIDTH)) goalTile -= SimulationScene::GRID_WIDTH;
        else if(SimulationScene::isPath(goalTile - 1) && !SimulationScene::wraps(goalTile, goalTile - 1)) goalTile -= 1;
        else if(SimulationScene::isPath(goalTile + SimulationScene::GRID_WIDTH)) goalTile += SimulationScene::GRID_WIDTH;
        else goalTile = nextTile;
    }
    nextTile = goalTile;

    /*if(!interract(currentTile + 1))
        if(!interract(currentTile - 1))
            if(!interract(currentTile + GRID_WIDTH))
                interract(currentTile - GRID_WIDTH);*/
    interract(currentTile + 1);
    interract(currentTile - 1);
    interract(currentTile + SimulationScene::GRID_WIDTH);
    interract(currentTile - SimulationScene::GRID_WIDTH);
}

bool LittleGuy::interract(int tile) {
    if(SimulationScene::wraps(currentTile, tile)) return false;
    if(tile < 0 || tile >= SimulationScene::GRID_WIDTH * SimulationScene::GRID_HEIGHT) return false;
    Team *team = SimulationScene::teams[teamIndex];
    for(int i = 0; i < team->teamGoalTileList.size(); i++) {
        if(team->teamGoalTileList[i] == tile) {
            if(SimulationScene::tiles[tile].type == GRASS) {
                if(team->coins >= SimulationScene::PATH_COST) {
                    team->coins -= SimulationScene::PATH_COST;
                    SimulationScene::tiles[tile].type = PATH;
                    team->getConnectedTiles();
                    team->teamGoalTileList.erase(team->teamGoalTileList.begin() + i);
                    return true;
                }
            } else if(SimulationScene::tiles[tile].type == TREE) {
                if(team->coins >= SimulationScene::TREE_COST) {
                    team->coins -= SimulationScene::TREE_COST;
                    SimulationScene::tiles[tile].type = GRASS;
                    team->getConnectedTiles();
                    return true;
                }
            }  else if(SimulationScene::tiles[tile].type == WATER) {
                if(team->coins >= SimulationScene::WATER_COST) {
                    team->coins -= SimulationScene::WATER_COST;
                    SimulationScene::tiles[tile].type = GRASS;
                    team->getConnectedTiles();
                    return true;
                }
            }

        }
    }

    switch(SimulationScene::tiles[tile].type) {
        case GRASS: return false;
        case PATH: return false;
        case GENERATOR: { //pickup
            if(item == NONE && SimulationScene::tiles[tile].data > 0) {
                item = GOLD;
                SimulationScene::tiles[tile].data--;
                return true;
            } break;
        }
        case SPAWNER:
            if(teamIndex != SimulationScene::tiles[tile].data) SimulationScene::tiles[tile].data = teamIndex;
        case BANK: { //deposit
            if(item == GOLD) {
                item = NONE;
                team->coins++;
                if(SimulationScene::tiles[tile].type == BANK) SimulationScene::tiles[tile].data++;
                return true;
            } break;
        }
        default: break;
    }
    return false;
}

void LittleGuy::takeDamage(float damage) {
    health -= damage;
    if(health <= 0) {
        exists = false;
        SimulationScene::teams[teamIndex]->guyCount--;
    }

}

void LittleGuy::update(float deltaTime) {

    maxHealth -= SimulationScene::GUY_MAX_HEALTH * SimulationScene::GUY_MAX_HEALTH_REDUCTION * deltaTime;

    if(health < maxHealth) {
        health += maxHealth * SimulationScene::GUY_HEALING * deltaTime;
    }
    health = min(health, maxHealth);
    if(health <= 0) {
        SimulationScene::teams[teamIndex]->guyCount--;
        exists = false;
    }

    vec2 speed = getSpeed() + vec2(currentTile - nextTile) * SimulationScene::SPEED_HEIGHT_MULT;
    speed.x = max(speed.x, SimulationScene::SPEED_MIN_HEIGHT);
    speed.y = max(speed.y, SimulationScene::SPEED_MIN_HEIGHT);
    int goalTile = nextTile;
    vec2 goal = vec2(goalTile % SimulationScene::GRID_WIDTH, goalTile / SimulationScene::GRID_WIDTH) * SimulationScene::TILE_DIMS + (SimulationScene::TILE_DIMS * 0.5f);// - (GUY_DIMS * guys[i].scale * 0.5f);
    /*if(glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_MIDDLE)) {
        goal = vec2(mx, my);
    }*/
    if(length(goal - vec2(x, y)) > speed.x * deltaTime) {
        float moveX = (x > goal.x ? -speed.x : speed.x) * deltaTime;
        float moveY = (y > goal.y ? -speed.y : speed.y) * deltaTime;
        x += moveX;
        y += moveY;
    } else {
        x = goal.x;
        y = goal.y;
        action();
    }

}

void LittleGuy::draw() const {
        //vec3 color = vec3(1.0f - (speed - MIN_SPEED) / (MAX_SPEED - MIN_SPEED));
        Texture2d::setColor(SimulationScene::teams[teamIndex]->color * 0.5f + vec3(1 - maxHealth / SimulationScene::GUY_MAX_HEALTH) * 0.8f);


        vec2 pos = vec2(x - (SimulationScene::GUY_DIMS.x * scale * 0.5f), y - (SimulationScene::GUY_DIMS.y * scale * 0.5f));
        SimulationScene::blank.draw(pos.x, pos.y, SimulationScene::GUY_DIMS.x * scale, SimulationScene::GUY_DIMS.y * scale);


        vec4 itemColor = vec4(0);

        switch(item) {
            default:
            case NONE: itemColor = vec4(0); break;
            case GOLD: itemColor = vec4(255, 184, 5, 255) / 255.0f; break;
        }
        //pos.x + (GUY_DIMS.x * guys[i].scale * ITEM_SCALE * 0.5f), pos.y + (GUY_DIMS.y * guys[i].scale * ITEM_SCALE * 0.5f)
        Circle2d item = Circle2d(vec2(x, y), SimulationScene::GUY_DIMS.x * scale * SimulationScene::ITEM_SCALE * 0.5f, 0, itemColor);
        item.draw(true);
        //blank.draw(pos.x + (GUY_DIMS.x * guys[i].scale * ITEM_SCALE * 0.5f), pos.y + (GUY_DIMS.y * guys[i].scale * ITEM_SCALE * 0.5f), GUY_DIMS.x * guys[i].scale * ITEM_SCALE, GUY_DIMS.y * guys[i].scale * ITEM_SCALE);
        //blank.draw(guys[i].x - ITEM_SCALE * 0.5f, guys[i].y - ITEM_SCALE * 0.5f, GUY_DIMS.x * guys[i].scale * ITEM_SCALE, GUY_DIMS.y * guys[i].scale * ITEM_SCALE);

        //debug lines
        //Line2d test = Line2d(vec2(x, y), goal, vec4(color, 1));
        //test.draw(true);

        //health bars
        Texture2d::setColor(vec4(1));
        SimulationScene::blank.draw(x - scale * SimulationScene::GUY_DIMS.x * 0.5f, y + scale * SimulationScene::GUY_DIMS.y * 0.6f, scale * SimulationScene::GUY_DIMS.x, scale * SimulationScene::GUY_DIMS.y * 0.2f);
        Texture2d::setColor(SimulationScene::teams[teamIndex]->color);
        SimulationScene::blank.draw(x - scale * SimulationScene::GUY_DIMS.x * 0.5f + 1, y + scale * SimulationScene::GUY_DIMS.y * 0.6f + 0.5f, (scale * SimulationScene::GUY_DIMS.x - (0.5f)) * health / maxHealth, scale * SimulationScene::GUY_DIMS.y * 0.2f - 1);
}

LittleGuy LittleGuy::createGuy(int tile) {
    LittleGuy guy = LittleGuy();
    guy.currentTile = tile;
    guy.nextTile = tile;
    guy.x = guy.currentTile % SimulationScene::GRID_WIDTH * SimulationScene::TILE_DIMS.x + SimulationScene::TILE_DIMS.x * 0.5f;
    guy.y = guy.currentTile / SimulationScene::GRID_WIDTH * SimulationScene::TILE_DIMS.y + SimulationScene::TILE_DIMS.y * 0.5f;
    guy.exists = true;
    guy.action();
    return guy;
}

vec2 LittleGuy::getSpeed() const {
    return SimulationScene::TILE_DIMS * (speed + (SimulationScene::MAX_SCALE - scale) * SimulationScene::SPEED_SCALE_MULT);
}