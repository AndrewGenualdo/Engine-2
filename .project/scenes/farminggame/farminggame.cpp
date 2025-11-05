//
// Created by cobble on 8/19/2025.
//

#include "farmingGame.h"
#include "littleGuy.h"
#include "tasks/TaskTravel.h"

Camera FarmingScene::camera = Camera();
Window *FarmingScene::window = nullptr;
FontRenderer FarmingScene::fontRenderer = FontRenderer();
Texture2d FarmingScene::blank = Texture2d();
//MultiTexture2d FarmingScene::landTextures = MultiTexture2d();
//MultiTexture2d FarmingScene::structureTextures = MultiTexture2d();

//Texture2d FarmingScene::guy = Texture2d();
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
    world = FarmingWorld("saves/farming.txt");
    FarmingWorld::landTilemap = Tiles2d("assets/farminggame/spritesheet.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, FarmingWorld::TEXTURE_TILE_COUNT, world.landData);
    FarmingWorld::plantTilemap = Tiles2d("assets/farminggame/plants.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, FarmingWorld::TEXTURE_TILE_COUNT, world.plantData);

    LittleGuy::setWorld(&world);
    LittleGuy::setTexture("assets/farminggame/guy.png", 50, 100);
    Window::setVsync(true);
}

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
    const ivec2 mouseTile = FarmingWorld::getTileFromPos(vec2(mx, my));

    if (window->isInputClicked(GLFW_KEY_F)) {
        world.guys[0].setTask(new TaskTravel(&world.guys[0], mouseTile));
    }

    world.update(deltaTime);
    world.draw();
    fontRenderer.draw("Pos: " + std::to_string(world.guys[0].getPos().x) + ", " + std::to_string(world.guys[0].getPos().y) + "\nTile: " + std::to_string(world.guys[0].getTile().x) + ", " + std::to_string(world.guys[0].getTile().y), 10, 10 + fontRenderer.getHeight() * fontScale, fontScale);


    const int mouseTileIndex = mouseTile.y * FarmingWorld::TILES_HORIZ + mouseTile.x;
    if (mouseTileIndex >= 0 && mouseTileIndex < FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT) {
        if (window->isInputClicked(GLFW_MOUSE_BUTTON_LEFT)) world.setLandData(mouseTileIndex, world.getLandData(mouseTileIndex) + 1);
        if (window->isInputClicked(GLFW_MOUSE_BUTTON_RIGHT)) world.setLandData(mouseTileIndex, world.getLandData(mouseTileIndex) - 1);
        if (world.getLandData(mouseTileIndex) > 100000000 ) world.setLandData(mouseTileIndex, 0);
        if (world.getLandData(mouseTileIndex) >= FarmingWorld::TEXTURE_TILE_COUNT * FarmingWorld::TEXTURE_TILE_COUNT) world.setLandData(mouseTileIndex, FarmingWorld::TEXTURE_TILE_COUNT * FarmingWorld::TEXTURE_TILE_COUNT - 1);
        Texture2d::setColor(vec4(1, 1, 1, 0.25f));
        blank.draw(FarmingWorld::TILE_OFFSET_X + mouseTile.x * FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_OFFSET_Y + mouseTile.y * FarmingWorld::TILE_HEIGHT, FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_HEIGHT, true);
    }
}

void FarmingScene::cleanup() {
    world.save();
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
