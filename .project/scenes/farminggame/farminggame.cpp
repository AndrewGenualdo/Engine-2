//
// Created by cobble on 8/19/2025.
//

#include "farmingGame.h"
#include "littleGuy.h"
#include "ew/ewMath/ewMath.h"
#include "items/Item.h"
#include "items/produce/ItemCarrot.h"
#include "items/seeds/ItemCarrotSeed.h"
#include "items/produce/ItemTomato.h"
#include "items/seeds/ItemTomatoSeed.h"
#include "tasks/TaskRetrieveItem.h"
#include "tasks/TaskTravel.h"
#include "tiles/plants/TilePlantTomato.h"

Camera FarmingScene::camera = Camera();
Window *FarmingScene::window = nullptr;
FontRenderer FarmingScene::fontRenderer = FontRenderer();
Texture2d FarmingScene::blank = Texture2d();
FarmingWorld *FarmingScene::world = nullptr;
float FarmingScene::lastTick = 0.0f;

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
    world = new FarmingWorld("saves/farming.txt");
    FarmingWorld::landTilemap = Tiles2d("assets/farminggame/spritesheet.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, FarmingWorld::TEXTURE_TILE_COUNT, world->landData);
    FarmingWorld::plantTilemap = Tiles2d("assets/farminggame/plants.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, FarmingWorld::TEXTURE_TILE_COUNT, world->plantData);
    LittleGuy::setTexture("assets/farminggame/guy.png", FarmingWorld::TILE_WIDTH * 0.5f, FarmingWorld::TILE_HEIGHT, 2);
    Item::setTexture("assets/farminggame/items.png", FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_HEIGHT, 64);
    Item::loadData();
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
        //world->guys[0]->setTask(new TaskTravel(world->guys[0], mouseTile));
        world->guys[0]->setTask(new TaskRetrieveItem(world->guys[0], FarmingObject::ObjectType::ITEM_TOMATO, 5));
    }
    if (window->isInputPressed(GLFW_KEY_P)) {
        for (int i = 0; i < FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT; i++) {
            bool isFarmland = true;//FarmingWorld::isFarmland(i);
            bool planted = false;
            if (isFarmland) {
                for (int j = 0; j < world->objects.size(); j++) {
                    FarmingObject *obj = world->objects[j];
                    auto *tomato = dynamic_cast<TilePlantTomato*>(obj);
                    if (tomato && tomato->tile.y * FarmingWorld::TILES_HORIZ + tomato->tile.x == i) {
                        planted = true;
                        break;
                    }
                }
            }
            if (!planted) {
                world->objects.push_back(new TilePlantTomato(ivec2(i % FarmingWorld::TILES_HORIZ, i / FarmingWorld::TILES_HORIZ), 4, 100, 4));
                break;
            }
        }
    }
    if (window->isInputClicked(GLFW_KEY_O)) {
        world->clearObjects();

    }

    if (window->isInputClicked(GLFW_KEY_I)) {
        Item *item = nullptr;
        int random = ew::RandomRange(0, 4);
        switch (random) {
            case 0: item = new ItemTomato(vec2(mx, my)); break;
            case 1: item = new ItemTomatoSeed(vec2(mx, my)); break;
            case 2: item = new ItemCarrot(vec2(mx, my)); break;
            case 3: item = new ItemCarrotSeed(vec2(mx, my)); break;
            default: item = new Item();
        }

        if (length(vec2(mx, my) - world->guys[0]->getPos()) < FarmingWorld::TILE_WIDTH) {
            world->guys[0]->giveItem(item);
        } else {
            world->objects.push_back(item);
        }
    }


    world->update(deltaTime);

    if (window->getTime() - lastTick >= 1.0f / FarmingWorld::TICKS_PER_SECOND) {
        lastTick += 1.0f / FarmingWorld::TICKS_PER_SECOND;
        world->tick();
    }

    world->draw();
    fontRenderer.draw("Pos: " + std::to_string(world->guys[0]->getPos().x) + ", " + std::to_string(world->guys[0]->getPos().y) + "\nTile: " + std::to_string(world->guys[0]->getTile().x) + ", " + std::to_string(world->guys[0]->getTile().y), 10, 10 + fontRenderer.getHeight() * fontScale, fontScale);


    const int mouseTileIndex = mouseTile.y * FarmingWorld::TILES_HORIZ + mouseTile.x;
    if (mx >= FarmingWorld::TILE_OFFSET_X && my >= FarmingWorld::TILE_OFFSET_Y && mouseTile.x < FarmingWorld::TILES_HORIZ && mouseTile.y < FarmingWorld::TILES_VERT) {
        Texture2d::setColor(vec4(1, 1, 1, 0.5f));
        blank.draw(FarmingWorld::TILE_OFFSET_X + mouseTile.x * FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_OFFSET_Y + mouseTile.y * FarmingWorld::TILE_HEIGHT, FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_HEIGHT, true);
    }

}

void FarmingScene::cleanup() {
    if (world != nullptr) {
        world->save();
        delete world;
        world = nullptr;
    }
}

void FarmingScene::keyPress(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_R) {
            cleanup();
            load();
        } else if(key == GLFW_KEY_F8) {
            debugMode = !debugMode;
        } else if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL) {
            world->save();
        }
    }
}

void FarmingScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}
