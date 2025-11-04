//
// Created by cobble on 8/19/2025.
//

#include "farmingGame.h"
#include "littleGuy.h"

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



void FarmingScene::load() {
    window->setWindowTitle("Farming Sim Scene");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");
    landTextures = MultiTexture2d("assets/farminggame/spritesheet.png", 64);
    structureTextures = MultiTexture2d("assets/farminggame/spritesheet2.png", 64);
    guy = Texture2d("assets/farminggame/guy.png", GL_NEAREST, GL_TEXTURE_WRAP_S);
    world = FarmingWorld(FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, "saves/farming.txt");
    LittleGuy::setWorld(&world);
    //tileData = new unsigned int[TILES_HORIZ * TILES_VERT];
    //for (int i = 0; i < TILES_HORIZ * TILES_VERT; i++) tileData[i] = ew::RandomRange(0, 6);
    tiles = Tiles2d("assets/farminggame/spritesheet.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, FarmingWorld::TEXTURE_TILE_COUNT, world.landData);
    structures = Tiles2d("assets/farminggame/spritesheet.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, FarmingWorld::TEXTURE_TILE_COUNT, world.structureData);
    Window::setVsync(true);
}

vec2 guyPos = vec2(100, 100);
int layer = 0;

void FarmingScene::draw() {
    const float deltaTime = window->update();
    camera.update(window->window, deltaTime);
    float fontScale = 3.0f;

    //do background stuff here
    Texture2d::setColor(vec4(38, 127, 0, 255) / 255.0f);
    blank.drawRaw(window->gx, window->gy, window->gw, window->gh, true);

    Texture2d::gameCamera.reset();
    Texture2d::gameCamera.expandToInclude(0, 0);
    Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);

    const float mx = window->mousePos.x;
    const float my = window->mousePos.y;

    if (glfwGetKey(window->window, GLFW_KEY_W)) guyPos.y += deltaTime * 200;
    if (glfwGetKey(window->window, GLFW_KEY_S) && !glfwGetKey(window->window, GLFW_KEY_LEFT_CONTROL)) guyPos.y -= deltaTime * 200;
    if (glfwGetKey(window->window, GLFW_KEY_A)) guyPos.x -= deltaTime * 200;
    if (glfwGetKey(window->window, GLFW_KEY_D)) guyPos.x += deltaTime * 200;

    const ivec2 mouseTile = FarmingWorld::getTileFromPos(vec2(mx, my));
    const int tileIndex = mouseTile.y * FarmingWorld::TILES_HORIZ + mouseTile.x;
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
            default: break;
        }

        if (world.getLandData(tileIndex) > 100000000 ) world.setLandData(tileIndex, 0);
        if (world.getLandData(tileIndex) >= FarmingWorld::TEXTURE_TILE_COUNT * FarmingWorld::TEXTURE_TILE_COUNT) world.setLandData(tileIndex, FarmingWorld::TEXTURE_TILE_COUNT * FarmingWorld::TEXTURE_TILE_COUNT - 1);
        if (window->isInputClicked(GLFW_KEY_J) || window->isInputClicked(GLFW_KEY_K)) {
            if (layer == 0) layer = 1;
            else layer = 0;
            std::cout << "layer: " << layer << std::endl;
        }
    }

    Texture2d::setColor(vec4(1));
    tiles.draw(FarmingWorld::TILE_OFFSET_X, FarmingWorld::TILE_OFFSET_Y, FarmingWorld::TILE_WIDTH * FarmingWorld::TILES_HORIZ, FarmingWorld::TILE_HEIGHT * FarmingWorld::TILES_VERT, true);
    //structures.draw(TILE_OFFSET_X, TILE_OFFSET_Y, TILE_WIDTH * FarmingWorld::TILES_HORIZ, TILE_HEIGHT * FarmingWorld::TILES_VERT, true);
    Texture2d::setColor(vec4(1, 1, 1, 0.25f));
    blank.draw(FarmingWorld::TILE_OFFSET_X + mouseTile.x * FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_OFFSET_Y + mouseTile.y * FarmingWorld::TILE_HEIGHT, FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_HEIGHT, true);
    Texture2d::setColor(vec4(1));
    guy.draw(guyPos.x, guyPos.y, 75, 150, true);
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
        } else if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL) {
            world.save();
        }
    }
}

void FarmingScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}
