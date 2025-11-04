//
// Created by cobble on 8/29/2025.
//

#ifndef BLOCK_H
#define BLOCK_H



class Block {
public:
    int type = 0;
    int data = 0;

    explicit Block(int type = 0, int data = 0);

    void draw(float x, float y, float w, float h) const;

    void setBeltTo(int dir);

    void setBeltFrom(int dir);

    void setRailType(int type);

};



#endif //BLOCK_H
