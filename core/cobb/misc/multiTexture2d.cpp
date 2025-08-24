//
// Created by cobble on 1/10/2025.
//

#include "multiTexture2d.h"

MultiTexture2d::MultiTexture2d() {
    m_path = "";
}

MultiTexture2d::MultiTexture2d(const std::string &path, int frames)  {
    this->m_frames = frames;
    float vertices[8] = {1, 1, 1, -1, -1, -1, -1, 1};
    loadVertices(vertices);
    m_path = path.c_str();
    m_filterMode = GL_NEAREST;
    m_wrapMode = GL_REPEAT;
    load(true);
}

float MultiTexture2d::getWidth() const  {
    return static_cast<float>(m_width) / static_cast<float>(m_frames);
}

float MultiTexture2d::getTotalWidth() const {
    return static_cast<float>(m_width);
}

int MultiTexture2d::getFrames() const {
    return m_frames;
}

void MultiTexture2d::loadShader() {
    if (multiTexture2dShader == nullptr) {
        multiTexture2dShader = new Shader("assets/multiTexture2d");
        multiTexture2dShader->use();
        multiTexture2dShader->setVec4("color", vec4(1));
        glBindVertexArray(*getVAO());
        std::cout << "Loaded multiTexture2d shader!" << std::endl;
    }
}

void MultiTexture2d::drawRaw(float x, float y, float width, float height, int frame, bool shouldBind) const {
    float w = width * 0.5f;
    float h = height * 0.5f;
    loadShader();
    glDisable(GL_DEPTH_TEST);
    if (shouldBind) bind();
    multiTexture2dShader->use();
    glBindVertexArray(*getVAO());
    multiTexture2dShader->setInt("frames", m_frames);
    multiTexture2dShader->setInt("frame", frame);
    multiTexture2dShader->setMat4("proj", orthoProj);
    multiTexture2dShader->setMat4("model", Object::translate(x + w / getWidth() * 0.5f + w * 0.5f, y + h / getHeight(), 0) * Object::scale(w, h, 1));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void MultiTexture2d::draw(float x, float y, float width, float height, int frame, bool shouldBind) const {
    //translates from world coordinates to game camera coordinates
    gameCamera.adjustToAspectRatio(Window::GAME_WIDTH / Window::GAME_HEIGHT);
    gameCamera.setMinWidth(100);
    float gsw = Window::GAME_WIDTH / gameCamera.w;
    float gsh = Window::GAME_HEIGHT / gameCamera.h;
    float gx = (x - gameCamera.x) * gsw;
    float gy = (y - gameCamera.y) * gsh;
    float gw = width * gsw;
    float gh = height * gsh;

    //translates from game camera coordinates to screen coordinates
    float scale = window->gw / Window::GAME_WIDTH;
    float nx = window->sX(gx) + (window->sX(0) * (gx / Window::GAME_WIDTH));
    float ny = window->sY(gy) + (window->sY(0) * (gy / Window::GAME_HEIGHT));
    float nw = gw * scale;
    float nh = gh * scale;
    drawRaw(nx, ny, nw, nh, frame, shouldBind);
}

void MultiTexture2d::drawRaw(float x, float y, float width, float height, float rotation, int frame, bool shouldBind) const {
    float w = width * 0.5f;
    float h = height * 0.5f;
    loadShader();
    glDisable(GL_DEPTH_TEST);
    if (shouldBind) bind();
    multiTexture2dShader->use();
    glBindVertexArray(*getVAO());
    multiTexture2dShader->setInt("frames", m_frames);
    multiTexture2dShader->setInt("frame", frame);
    multiTexture2dShader->setMat4("proj", orthoProj);
    multiTexture2dShader->setMat4("model", Object::translate(x + w / getWidth() * 0.5f + w * 0.5f, y + h / getHeight(), 0) * Object::rotate(0, 0, rotation) * Object::scale(w, h, 1));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void MultiTexture2d::draw(float x, float y, float width, float height, float rotation, int frame, bool shouldBind) const {
    //translates from world coordinates to game camera coordinates
    gameCamera.adjustToAspectRatio(Window::GAME_WIDTH / Window::GAME_HEIGHT);
    gameCamera.setMinWidth(100);
    float gsw = Window::GAME_WIDTH / gameCamera.w;
    float gsh = Window::GAME_HEIGHT / gameCamera.h;
    float gx = (x - gameCamera.x) * gsw;
    float gy = (y - gameCamera.y) * gsh;
    float gw = width * gsw;
    float gh = height * gsh;

    //translates from game camera coordinates to screen coordinates
    float scale = window->gw / Window::GAME_WIDTH;
    float nx = window->sX(gx) + (window->sX(0) * (gx / Window::GAME_WIDTH));
    float ny = window->sY(gy) + (window->sY(0) * (gy / Window::GAME_HEIGHT));
    float nw = gw * scale;
    float nh = gh * scale;
    drawRaw(nx, ny, nw, nh, rotation, frame, shouldBind);
}
