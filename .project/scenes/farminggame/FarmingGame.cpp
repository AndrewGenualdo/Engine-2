//
// Created by cobble on 8/19/2025.
//

#include "FarmingGame.h"

#include "cobb/misc/profiler.h"
#include "misc/LittleGuy.h"
#include "ew/ewMath/ewMath.h"
#include "items/Item.h"
#include "misc/LittleGuyManager.h"
#include "misc/Truck.h"
#include "tasks/TaskFetchItem.h"

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
    Profiler::create("load");
    Profiler::get("load").start();
    window->setWindowTitle("Farming Sim Scene");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");
    FarmingObject::loadData();
    world = new FarmingWorld("saves/farming.txt");
    FarmingObject::loadInventory();
    FarmingWorld::landTilemap = Tiles2d("assets/farminggame/spritesheet.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, FarmingWorld::TEXTURE_TILE_COUNT, world->landData);
    FarmingWorld::plantTilemap = Tiles2d("assets/farminggame/plants.png", FarmingWorld::TILES_HORIZ, FarmingWorld::TILES_VERT, FarmingWorld::TEXTURE_TILE_COUNT, world->plantData);
    FarmingWorld::barnTexture = Texture2d("assets/farminggame/barn.png", GL_NEAREST, GL_TEXTURE_WRAP_S);
    Truck::texture = MultiTexture2d("assets/farminggame/truck.png", 5);
    FarmingWorld::uiTexture = MultiTexture2d("assets/farminggame/ui.png", 64);
    FarmingWorld::fontRenderer = &fontRenderer;
    LittleGuy::setTexture("assets/farminggame/guy.png", static_cast<int>(static_cast<float>(FarmingWorld::TILE_WIDTH) * 0.5f), FarmingWorld::TILE_HEIGHT, 12);
    Item::setTexture("assets/farminggame/items.png", FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_HEIGHT, 64);
    Window::setVsync(true);
    lastTick = window->getTime();
    Profiler::get("load").end();
    Profiler::create("tick");
    Profiler::create("update");
    Profiler::create("draw");
}

void FarmingScene::draw() {
    float deltaTime = window->update();
    camera.update(window->window, deltaTime);
    float fontScale = 3.0f;

    //do background stuff here
    Texture2d::setColor(vec4(38, 127, 0, 255) / 255.0f);
    blank.drawRaw(window->gx, window->gy, window->gw, window->gh, true);

    Texture2d::gameCamera.reset();
    Texture2d::gameCamera.expandToInclude(0, 0);
    Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);

    if (window->isInputPressed(GLFW_KEY_SPACE)) { if (window->isInputPressed(GLFW_KEY_LEFT_CONTROL)) { deltaTime *= 9; } else {deltaTime *= 3;} }

    const float mx = window->mousePos.x;
    const float my = window->mousePos.y;
    const ivec2 mouseTile = FarmingWorld::getTileFromPos(vec2(mx, my));

    //https://emscripten.org/docs/getting_started/downloads.html

    if (window->isInputClicked(GLFW_KEY_O)) {
        FarmingObject::cleanData();
        load();
        world->clear();
        FarmingObject::loadInventory();
    }
    if (window->isInputClicked(GLFW_KEY_F)) {
        world->guyManager->setGoal(FarmingObject::TypeID::ITEM_PRODUCE_TOMATO, FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT);
    }
    else if (window->isInputClicked(GLFW_KEY_G)) {
        world->guyManager->setGoal(FarmingObject::TypeID::ITEM_PRODUCE_CARROT, FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT);
    }
    else if (window->isInputClicked(GLFW_KEY_H)) {
        world->guyManager->setGoal(FarmingObject::TypeID::ITEM_PRODUCE_BLUEBERRY, FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT * FarmingObject::getData<TilePlant::PlantData>(FarmingObject::TypeID::TILE_PLANT_BLUEBERRY)->amountProduces);
    }
    if (window->isInputClicked(GLFW_KEY_U)) {
        for (int i = 0; i < 5; i++) {
            world->guyManager->addGuy(new LittleGuy(mouseTile));
        }
    }
    if (window->isInputPressed(GLFW_KEY_D)) {
        for (int i = 0; i < FarmingWorld::TILES_HORIZ * FarmingWorld::TILES_VERT; i++) {
            if (world->getTile(i)->exists()) continue;
            float rand = ew::RandomRange(0, 1);
            std::vector<FarmingObject::TypeID> plants;
            plants.push_back(FarmingObject::TypeID::TILE_PLANT_TOMATO);
            plants.push_back(FarmingObject::TypeID::TILE_PLANT_CARROT);
            plants.push_back(FarmingObject::TypeID::TILE_PLANT_BLUEBERRY);
            for (float j = 0; j < plants.size(); j++) {
                if (rand < (j + 1) / plants.size()) {
                    ivec2 tile = ivec2(i % FarmingWorld::TILES_HORIZ, i / FarmingWorld::TILES_HORIZ);
                    world->setTile(tile, FarmingWorld::createTile(plants[static_cast<int>(j)], tile));
                    break;
                }
            }
            //if (ew::RandomRange(0, 1) > 0.5f) world->setTile(ivec2(i % FarmingWorld::TILES_HORIZ, i / FarmingWorld::TILES_HORIZ), new TilePlantCarrot(ivec2(i % FarmingWorld::TILES_HORIZ, i / FarmingWorld::TILES_HORIZ)));
            //else world->setTile(ivec2(i % FarmingWorld::TILES_HORIZ, i / FarmingWorld::TILES_HORIZ), new TilePlantTomato(ivec2(i % FarmingWorld::TILES_HORIZ, i / FarmingWorld::TILES_HORIZ)));
            break;
        }
    }
    if (window->isInputClicked(GLFW_KEY_L)) {
        world->logs = !world->logs;
    }
    if (window->isInputClicked(GLFW_KEY_M)) {
        world->truck.enter();
    }

    bool isTick = false;
    if (world->time - lastTick >= 1.0f / FarmingWorld::TICKS_PER_SECOND) {
        lastTick += 1.0f / FarmingWorld::TICKS_PER_SECOND;
        isTick = true;
    }
    Profiler::get("tick").start();
    world->tick(isTick, deltaTime);
    Profiler::get("tick").end();
    Profiler::get("update").start();
    world->update(deltaTime);
    Profiler::get("update").end();
    Profiler::get("draw").start();
    world->draw();
    Profiler::get("draw").end();
    //if(!world->guys.empty()) fontRenderer.draw("Pos: " + std::to_string(world->guys[0]->getPos().x) + ", " + std::to_string(world->guys[0]->getPos().y) + "\nTile: " + std::to_string(world->guys[0]->getTile().x) + ", " + std::to_string(world->guys[0]->getTile().y), 10, 10 + fontRenderer.getHeight() * fontScale, fontScale);


    const int mouseTileIndex = mouseTile.y * FarmingWorld::TILES_HORIZ + mouseTile.x;
    if (mx >= FarmingWorld::TILE_OFFSET_X && my >= FarmingWorld::TILE_OFFSET_Y && mouseTile.x < FarmingWorld::TILES_HORIZ && mouseTile.y < FarmingWorld::TILES_VERT) {
        Texture2d::setColor(vec4(1, 1, 1, 0.5f));
        blank.draw(static_cast<float>(FarmingWorld::TILE_OFFSET_X + mouseTile.x * FarmingWorld::TILE_WIDTH), static_cast<float>(FarmingWorld::TILE_OFFSET_Y + mouseTile.y * FarmingWorld::TILE_HEIGHT), FarmingWorld::TILE_WIDTH, FarmingWorld::TILE_HEIGHT, true);

        if (debugMode) {
            std::string tileInfo = "Tile: " + std::to_string(mouseTile.x) + ", " + std::to_string(mouseTile.y) + "\n";
            tileInfo += "Type: " + FarmingObject::getData<FarmingObject::ObjectData>(world->getTile(mouseTile)->getType())->configKey + "\n";
            const bool exists = world->getTile(mouseTile)->exists();
            tileInfo += std::string("Exists: ") + (exists ? "true" : "false") + "\n";
            tileInfo += std::string("Being Used: ") + (world->getTile(mouseTile)->isBeingUsed() ? "true" : "false");

            Texture2d::setColor(vec4(0, 0, 0, 0.5f));
            const float tileInfoBgX = Window::GAME_WIDTH - fontRenderer.getWidth(tileInfo) * fontScale - 20;
            const float tileInfoBgY = Window::GAME_HEIGHT - fontRenderer.getHeight() * 5 * fontScale - 20;
            blank.draw(tileInfoBgX, tileInfoBgY, Window::GAME_WIDTH - tileInfoBgX, Window::GAME_HEIGHT - tileInfoBgY, true);
            fontRenderer.setColor(vec3(1, 0.5f, 0.5f));
            fontRenderer.draw(tileInfo, Window::GAME_WIDTH - fontRenderer.getWidth(tileInfo) * fontScale - 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);

            std::string taskInfo;
            for (const auto &[delay, tasks] : world->guyManager->tasks) {
                if (tasks.empty()) continue;
                if (tasks[0]->getGuy() == nullptr) continue;
                if (tasks[0]->getGuy()->tile == mouseTile) {
                    taskInfo += "Task: " + tasks[0]->getName();
                    break;
                }
            }
            if (!taskInfo.empty()) {
                const float taskInfoBgX = Window::GAME_WIDTH - fontRenderer.getWidth(taskInfo) * fontScale - 20;
                const float taskInfoBgY = Window::GAME_HEIGHT - 20 - fontRenderer.getHeight() * 8 * fontScale;
                blank.draw(taskInfoBgX, taskInfoBgY, Window::GAME_WIDTH - taskInfoBgX, fontRenderer.getHeight() * fontScale + 20, true);

                fontRenderer.setColor(vec3(0.5f, 0.5f, 1.0f));
                fontRenderer.draw(taskInfo, Window::GAME_WIDTH - fontRenderer.getWidth(taskInfo) * fontScale - 10, Window::GAME_HEIGHT - 10 - fontRenderer.getHeight() * 8 * fontScale, fontScale);
            }
        }

    }
    if (debugMode) {
        std::string profilerInfo;
        int profilerLines = 2;
        int total = 0;
        for (const auto&[fst, snd] : Profiler::getMap()) if (fst != "load") total += static_cast<int>(Profiler::get(fst).duration_micro());
        profilerInfo += "Load: " + ew::formatValue(static_cast<int>(Profiler::get("load").duration_micro())) + "ns\n";
        for (const auto&[fst, snd] : Profiler::getMap()) {
            if (fst == "load") continue;
            int micro = static_cast<int>(Profiler::get(fst).duration_micro());
            profilerInfo += fst + ": " + ew::formatValue(micro) + "ns (" + std::to_string(static_cast<int>(static_cast<float>(micro) * 100.0f / static_cast<float>(total))) + "%)\n";
            profilerLines++;
        }
        profilerInfo += "Total: " + ew::formatValue(total) + "ns\n";
        Texture2d::setColor(vec4(0, 0, 0, 0.5f));
        blank.draw(Window::GAME_WIDTH - fontRenderer.getWidth(profilerInfo) * fontScale - 20, 0, fontRenderer.getWidth(profilerInfo) * fontScale + 20, fontRenderer.getHeight() * fontScale * static_cast<float>(profilerLines + 1) + 20);
        fontRenderer.setColor(vec3(0.5f, 1.0f, 0.5f));
        fontRenderer.draw(profilerInfo, Window::GAME_WIDTH - fontRenderer.getWidth(profilerInfo) * fontScale - 10, 10 + static_cast<float>(profilerLines) * fontScale * fontRenderer.getHeight(), fontScale);
    }

}

void FarmingScene::cleanup() {
    if (world != nullptr) {
        world->save();
        delete world;
        world = nullptr;
    }
    FarmingObject::cleanData();
}

void FarmingScene::keyPress(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_R) {
            cleanup();
            load();
        } else if(key == GLFW_KEY_F3) {
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
