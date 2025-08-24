//
// Created by cobble on 7/11/2025.
//

#include "circle2d.h"

Shader *Circle2d::circle2dShader = nullptr;

unsigned int * Circle2d::getVAO() {
    if (CIRCLE_2D_VAO == -1) {
        glGenVertexArrays(1, &CIRCLE_2D_VAO);
    }
    return &CIRCLE_2D_VAO;
}

unsigned int * Circle2d::getVBO() {
    if (CIRCLE_2D_VBO == -1) {
        glGenBuffers(1, &CIRCLE_2D_VBO);
    }

    return &CIRCLE_2D_VBO;
}

unsigned int * Circle2d::getEBO() {
    if(CIRCLE_2D_EBO == -1) {
        glGenBuffers(1, &CIRCLE_2D_EBO);
    }
    return &CIRCLE_2D_EBO;
}

