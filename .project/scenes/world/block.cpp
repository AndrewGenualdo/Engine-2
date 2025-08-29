//
// Created by cobble on 8/29/2025.
//

#include "block.h"

#include "world.h"

Block::Block(int type, int data) {
    this->type = type;
    this->data = data;
}

void Block::draw(float x, float y, float w, float h) const  {
    //default = 0
    //north = 1
    //east = 2
    //south = 3
    //west = 4
    //EX: default -> west = 04, EX2: south -> east = 32
    if(type == BELT) WorldScene::blocks[BELT_OFFSET + data].draw(x, y, w, h);
    else if(type == RAIL) WorldScene::blocks[RAIL_OFFSET + data].draw(x, y, w, h);
    else WorldScene::blocks[type].draw(x, y, w, h);
}

void Block::setBeltTo(int dir)  {
    int from = data / 10;
    if(dir != from) data = from * 10 + dir;
}

void Block::setBeltFrom(int dir) {
    int to = data % 10;
    if(dir != to) data = dir * 10 + to;
}

void Block::setRailType(int type) {
    data = type;
}
