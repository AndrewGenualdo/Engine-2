//
// Created by cobble on 7/10/2025.
//

#include "line2d.h"

Shader *Line2d::line2dShader = nullptr;

unsigned int * Line2d::getVAO() {
    if (LINE_2D_VAO == -1) {
        glGenVertexArrays(1, &LINE_2D_VAO);
    }

    return &LINE_2D_VAO;
}

unsigned int * Line2d::getVBO() {
    if (LINE_2D_VBO == -1) {
        glGenBuffers(1, &LINE_2D_VBO);
    }

    return &LINE_2D_VBO;
}
