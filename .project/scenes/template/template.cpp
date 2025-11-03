//
// Created by cobble on 8/19/2025.
//

#include "template.h"

Camera TemplateScene::camera = Camera();
Window *TemplateScene::window = nullptr;
FontRenderer TemplateScene::fontRenderer = FontRenderer();
Texture2d TemplateScene::blank = Texture2d();
bool TemplateScene::debugMode = false;

TemplateScene::TemplateScene(Window *w) {
    window = w;
}

TemplateScene::~TemplateScene() {
    cleanup();
}

void TemplateScene::load() {
    window->setWindowTitle("Template Scene");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");
}

void TemplateScene::draw() {
    float deltaTime = window->update();
    camera.update(window->window, deltaTime);
    float fontScale = 3.0f;

    //do background stuff here
    Texture2d::setColor(vec4(0, 0, 0, 1));
    blank.drawRaw(window->gx, window->gy, window->gw, window->gh, true);

    Texture2d::gameCamera.reset();
    Texture2d::gameCamera.expandToInclude(0, 0);
    Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);

    float mx = window->mousePos.x;
    float my = window->mousePos.y;

    Texture2d::setColor(vec4(1.0f, 0.5f, 0.5f, 1.0f));
    blank.draw(mx - 50, my - 50, 100.0f, 100.0f);

}

void TemplateScene::cleanup() {

}

void TemplateScene::keyPress(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_R) {
            cleanup();
            load();
        } else if(key == GLFW_KEY_F8) {
            debugMode = !debugMode;
        }
    }
}

void TemplateScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}
