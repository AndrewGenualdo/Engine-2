//
// Created by cobble on 11/3/2025.
//

#include "tiles2d.h"

Tiles2d::Tiles2d() {
    m_path = "";
}

Tiles2d::Tiles2d(const std::string &path, int width, int height, int *data) {
    this->m_width = width;
    this->m_height = height;
    this->m_data = data;
    float vertices[8] = {1, 1, 1, -1, -1, -1, -1, 1};
    loadVertices(vertices);
    m_path = path.c_str();
    m_filterMode = GL_NEAREST;
    m_wrapMode = GL_REPEAT;
    load(true);
}

void Tiles2d::draw(const float x, const float y, const float width, const float height, const bool shouldBind) const {
    //translates from world coordinates to game camera coordinates
    const float gsw = Window::GAME_WIDTH / gameCamera.w;
    const float gsh = Window::GAME_HEIGHT / gameCamera.h;
    const float gx = (x - gameCamera.x) * gsw;
    const float gy = (y - gameCamera.y) * gsh;
    const float gw = width * gsw;
    const float gh = height * gsh;

    //translates from game camera coordinates to screen coordinates
    const float scale = window->gw / Window::GAME_WIDTH;
    const float nx = window->sX(gx) + window->sX(0) * (gx / Window::GAME_WIDTH) + static_cast<float>(m_width) / width;
    const float ny = window->sY(gy) + window->sY(0) * (gy / Window::GAME_HEIGHT) + static_cast<float>(m_height) / height;
    const float nw = gw * scale;
    const float nh = gh * scale;

    drawRaw(nx, ny, nw, nh, shouldBind);
}

void Tiles2d::drawRaw(const float x, const float y, const float width, const float height, const bool shouldBind) const {
    const float w = width * 0.5f;
    const float h = height * 0.5f;
    loadShader();
    glDisable(GL_DEPTH_TEST);
    if (shouldBind) bind();
    tiles2dShader->use();
    glBindVertexArray(*getVAO());
    if (shouldBind) tiles2dShader->setMat4("proj", orthoProj);
    if (shouldBind) tiles2dShader->setInt("width", m_width);
    if (shouldBind) tiles2dShader->setInt("height", m_height);
    if (shouldBind) tiles2dShader->setIntArray64("tiles", m_data);
    tiles2dShader->setMat4("model", Object::translate(x + w, y + h, 0) * Object::scale(w, h, 1));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void Tiles2d::loadShader() {
    if (tiles2dShader == nullptr) {
        tiles2dShader = new Shader("assets/tiles2d");
        tiles2dShader->use();
        tiles2dShader->setVec4("color", vec4(1));
        glBindVertexArray(*getVAO());
        std::cout << "Loaded tiles2d shader!" << std::endl;
    }
}
