//
// Created by cobble on 8/18/2025.
//

#include "spline2d.h"

unsigned int * cobb::Spline2d::getVAO() {
    if(SPLINE_2D_VAO == -1) {
        glGenVertexArrays(1, &SPLINE_2D_VAO);
    }
    return &SPLINE_2D_VAO;
}

unsigned int * cobb::Spline2d::getVBO() {
    if(SPLINE_2D_VBO == -1) {
        glGenBuffers(1, &SPLINE_2D_VBO);
    }
    return &SPLINE_2D_VBO;
}

unsigned int * cobb::Spline2d::getEBO() {
    if(SPLINE_2D_EBO == -1) {
        glGenBuffers(1, &SPLINE_2D_EBO);
    }
    return &SPLINE_2D_EBO;
}
