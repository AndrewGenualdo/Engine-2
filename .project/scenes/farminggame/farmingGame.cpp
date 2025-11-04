//
// Created by cobble on 8/19/2025.
//

#include "farmingGame.h"

Camera FarmingScene::camera = Camera();
Window *FarmingScene::window = nullptr;
FontRenderer FarmingScene::fontRenderer = FontRenderer();
Texture2d FarmingScene::blank = Texture2d();
MultiTexture2d FarmingScene::landTextures = MultiTexture2d();
MultiTexture2d FarmingScene::structureTextures = MultiTexture2d();
Tiles2d FarmingScene::tiles = Tiles2d();
Tiles2d FarmingScene::structures = Tiles2d();
Texture2d FarmingScene::guy = Texture2d();
FarmingWorld FarmingScene::world = FarmingWorld();

bool FarmingScene::debugMode = false;

FarmingScene::FarmingScene(Window *w) {
    window = w;
}

FarmingScene::~FarmingScene() {
    cleanup();
}

constexpr int TILE_OFFSET_X = 100;
constexpr int TILE_OFFSET_Y = 100;
constexpr int TILE_WIDTH = 50;
constexpr int TILE_HEIGHT = 50;

constexpr int TEXTURE_TILE_COUNT = 64;

void FarmingScene::load() {
    window->setWindowTitle("Farming Sim Scene");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");
    landTextures = MultiTexture2d("assets/farminggame/spritesheet.png", 64);
    structureTextures = MultiTexture2d("assets/farminggame/spritesheet2.png", 64);
    guy = Texture2d("assets/farminggame/guy.png", GL_NEAREST, GL_TEXTURE_WRAP_S);
    world = FarmingWorld(FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, "saves/farming.txt");

    //tileData = new unsigned int[TILES_HORIZ * TILES_VERT];
    //for (int i = 0; i < TILES_HORIZ * TILES_VERT; i++) tileData[i] = ew::RandomRange(0, 6);
    tiles = Tiles2d("assets/farminggame/spritesheet.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, TEXTURE_TILE_COUNT, world.landData);
    structures = Tiles2d("assets/farminggame/spritesheet.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, TEXTURE_TILE_COUNT, world.structureData);
    Window::setVsync(true);
}

vec2 guyPos = vec2(100, 100);
int layer = 0;

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

    if (glfwGetKey(window->window, GLFW_KEY_W)) guyPos.y += deltaTime * 200;
    if (glfwGetKey(window->window, GLFW_KEY_S)) guyPos.y -= deltaTime * 200;
    if (glfwGetKey(window->window, GLFW_KEY_A)) guyPos.x -= deltaTime * 200;
    if (glfwGetKey(window->window, GLFW_KEY_D)) guyPos.x += deltaTime * 200;

    int tileX = (mx - TILE_OFFSET_X) / TILE_WIDTH;
    int tileY = (my - TILE_OFFSET_Y) / TILE_HEIGHT;
    int tileIndex = tileY * FarmingWorld::TILES_HORIZ + tileX;
    if (tileIndex >= 0 && tileIndex < FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT) {
        switch (layer) {
            case 0: {
                if (window->isInputClicked(GLFW_MOUSE_BUTTON_LEFT)) world.setLandData(tileIndex, world.getLandData(tileIndex) + 1);
                if (window->isInputClicked(GLFW_MOUSE_BUTTON_RIGHT)) world.setLandData(tileIndex, world.getLandData(tileIndex) - 1);
                break;
            }
            case 1: {

                break;
            }
        }

        if (world.getLandData(tileIndex) == 255 ) world.setLandData(tileIndex, 0);
        if (world.getLandData(tileIndex) > TEXTURE_TILE_COUNT) world.setLandData(tileIndex, TEXTURE_TILE_COUNT);
        if (window->isInputClicked(GLFW_KEY_UP) || window->isInputClicked(GLFW_KEY_DOWN)) {
            if (layer == 0) layer = 1;
            else layer = 0;
        }
    }

    Texture2d::setColor(vec4(1));
    tiles.draw(TILE_OFFSET_X, TILE_OFFSET_Y, TILE_WIDTH * FarmingWorld::TILES_HORIZ, TILE_HEIGHT * FarmingWorld::TILES_VERT, true);
    structures.draw(TILE_OFFSET_X, TILE_OFFSET_Y, TILE_WIDTH * FarmingWorld::TILES_HORIZ, TILE_HEIGHT * FarmingWorld::TILES_VERT, true);
    Texture2d::setColor(vec4(1, 1, 1, 0.25f));
    switch (layer) {
        case 0: landTextures.draw(TILE_OFFSET_X + tileX * TILE_WIDTH, TILE_OFFSET_Y + tileY * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, world.landData[tileIndex] + 1, true); break;
        case 1: landTextures.draw(TILE_OFFSET_X + tileX * TILE_WIDTH, TILE_OFFSET_Y + tileY * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, world.structureData[tileIndex] + 1, true); break;
    }
    //blank.draw(TILE_OFFSET_X + tileX * TILE_WIDTH, TILE_OFFSET_Y + tileY * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, true);
    //Texture2d::setColor(vec4(1));
    //guy.draw(guyPos.x, guyPos.y, 75, 150, true);
}

void FarmingScene::cleanup() {

}

void FarmingScene::keyPress(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_R) {
            cleanup();
            load();
        } else if(key == GLFW_KEY_F8) {
            debugMode = !debugMode;
        } else if (key == GLFW_KEY_S) {
            world.save();
        }
    }
}

void FarmingScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}
