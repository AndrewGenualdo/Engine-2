//
// Created by cobble on 8/19/2025.
//

#include "farminggame.h"

#include "ew/ewMath/ewMath.h"


Camera FarmingScene::camera = Camera();
Window *FarmingScene::window = nullptr;
FontRenderer FarmingScene::fontRenderer = FontRenderer();
Texture2d FarmingScene::blank = Texture2d();
MultiTexture2d FarmingScene::spritesheet = MultiTexture2d();
Tiles2d FarmingScene::tiles = Tiles2d();
int *FarmingScene::tileData = nullptr;
bool FarmingScene::debugMode = false;

FarmingScene::FarmingScene(Window *w) {
    window = w;
}

FarmingScene::~FarmingScene() {
    cleanup();
}

void FarmingScene::load() {
    window->setWindowTitle("Farming Sim Scene");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");
    spritesheet = MultiTexture2d("assets/farminggame/spritesheet.png", 64);

    tileData = new int[64];
    for (int i = 0; i < 64; i++) {
        tileData[i] = ew::RandomRange(0, 6);
    }
    tiles = Tiles2d("assets/farminggame/spritesheet.png", 8, 8, tileData);
    Window::setVsync(false);
}

void FarmingScene::draw() {
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

    //Texture2d::setColor(vec4(1.0f, 0.5f, 0.5f, 1.0f));
    //blank.draw(mx - 50, my - 50, 100.0f, 100.0f);

    //spritesheet.draw(mx - 50.0f, my - 50.0f, 100.0f, 100.0f, static_cast<int>(window->getTime()), true);
    tiles.draw(100, 100, 500, 500, true);
}

void FarmingScene::cleanup() {
    if (tileData != nullptr) {
        delete [] tileData;
        tileData = nullptr;
    }
}

void FarmingScene::keyPress(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_R) {
            cleanup();
            load();
        } else if(key == GLFW_KEY_F8) {
            debugMode = !debugMode;
        }
    }
}

void FarmingScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}
