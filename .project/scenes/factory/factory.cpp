//
// Created by cobble on 8/19/2025.
//

#include "factory.h"

#include "structures/bouncer.h"
#include "structures/track.h"
#include "structures/wall.h"


Camera FactoryScene::camera = Camera();
Window *FactoryScene::window = nullptr;
FontRenderer FactoryScene::fontRenderer = FontRenderer();
Texture2d FactoryScene::blank = Texture2d();
bool FactoryScene::debugMode = false;

std::vector<Item*> FactoryScene::items = std::vector<Item*>();
std::vector<Item*> FactoryScene::itemsToDestroy = std::vector<Item*>();
Structure ***FactoryScene::structs = nullptr;

int FactoryScene::score = 0;

Texture2d FactoryScene::wallpaper = Texture2d();

FactoryScene::FactoryScene(Window *w) {
    window = w;
}

FactoryScene::~FactoryScene() {
    cleanup();
}

void FactoryScene::load() {
    window->setWindowTitle("Vertical Factory?");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");

    wallpaper = Texture2d("assets/textures/ui/wallpaper.jpg");


    structs = new Structure**[static_cast<int>(GRID_DIMS.y)];
    for(int y = 0; y < GRID_DIMS.y; y++) {
        structs[y] = new Structure*[static_cast<int>(GRID_DIMS.x)];
        for(int x = 0; x < GRID_DIMS.x; x++) {
            structs[y][x] = nullptr;
        }
    }
    //for(int i = 0; i < GRID_DIMS.y; i++) structs[i] = new Structure[static_cast<int>(GRID_DIMS.x)];

    Collector().setTexture("assets/textures/ui/test.png");
    Miner().setTexture("assets/textures/ui/miner.png");
    Rock().setTexture("assets/textures/ui/captain_falcon.png");
    Bouncer().setTexture("assets/textures/ui/bouncer_left.png");
    Bouncer tempBouncer = Bouncer();
    tempBouncer.isLeft = false;
    tempBouncer.setTexture("assets/textures/ui/bouncer_right.png");
    Wall().setTexture("assets/textures/ui/wall_vertical.png");
    Wall tempWall = Wall();
    tempWall.isVertical = false;
    tempWall.setTexture("assets/textures/ui/wall_horizontal.png");
    float trackSpeed = 0.05f;
    Track::trackEndLeft = Animation("assets/textures/ui/tracks/end_left.png", 8, trackSpeed, true);
    Track::trackBasic = Animation("assets/textures/ui/tracks/basic.png", 8, trackSpeed, true);
    Track::trackEndRight = Animation("assets/textures/ui/tracks/end_right.png", 8, trackSpeed, true);
    Track::trackBasicToDiagonalRight = Animation("assets/textures/ui/tracks/basic_to_diagonal_right.png", 8, trackSpeed, true);
    Track::trackDiagonalRight = Animation("assets/textures/ui/tracks/diagonal_right.png", 8, trackSpeed, true);

    
    score = 0;
}


bool wasLeftMouseDown = false;
bool wasRightMouseDown = false;
bool wasMiddleMouseDown = false;

void FactoryScene::draw() {
    float deltaTime = window->update();
    camera.update(window->window, deltaTime);
    float fontScale = 3.0f;

    //do background stuff here
    Texture2d::setColor(vec4(1, 1, 1, 1));
    blank.drawRaw(window->gx, window->gy, window->gw, window->gh, true);



    Texture2d::gameCamera.reset();
    Texture2d::gameCamera.expandToInclude(0, 0);
    Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);

    Texture2d::setColor(vec4(1, 1, 1, 1));
    //wallpaper.drawRaw(window->gx + 320, window->gy + 362, window->gw, window->gh, true);
    wallpaper.draw(479, 540, Window::GAME_WIDTH, Window::GAME_HEIGHT, true);

    bool isLeftMouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT);
    bool isLeftMouseClick = isLeftMouseDown && !wasLeftMouseDown;
    wasLeftMouseDown = isLeftMouseDown;

    bool isRightMouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_RIGHT);
    bool isRightMouseClick = isRightMouseDown && !wasRightMouseDown;
    wasRightMouseDown = isRightMouseDown;

    bool isMiddleMouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_MIDDLE);
    bool isMiddleMouseClick = isMiddleMouseDown && !wasMiddleMouseDown;
    wasMiddleMouseDown = isMiddleMouseDown;

    float mx = window->mousePos.x;
    float my = window->mousePos.y;
    vec2 mouseTile = getTileAtPos(vec2(mx, my));

    /*if(isLeftMouseClick && isTile(mouseTile)) {
        int tileFull = isTileFull(mouseTile);
        if(tileFull != -1) {
            std::string name = structs[structs.size() - 1]->name();
            delete structs[tileFull];
            if(name == "Miner") structs[tileFull] = new Collector(mouseTile);
            //else if(name == "Collector") structs[tileFull] = new Miner(mouseTile);
            else if(name == "Collector") structs.erase(structs.begin() + tileFull);
        } else {
            structs.push_back(new Miner(mouseTile));
        }
    }*/
    if(isLeftMouseClick && isTile(mouseTile)) {
        if(getTile(mouseTile) != nullptr) {
            std::string name = getTile(mouseTile)->name();
            destroyTile(mouseTile);
            if(name == "Miner") setTile(mouseTile, new Collector(mouseTile));
            else if(name == "Collector") setTile(mouseTile, new Bouncer(mouseTile));
            else if(name == "Bouncer - Left") {
                setTile(mouseTile, new Bouncer(mouseTile));
                dynamic_cast<Bouncer*>(getTile(mouseTile))->isLeft = false;
            }
        } else {
            setTile(mouseTile, new Miner(mouseTile));
        }
    }

    if(isRightMouseClick && isTile(mouseTile)) {
        if(getTile(mouseTile) != nullptr) {
            std::string name = getTile(mouseTile)->name();
            destroyTile(mouseTile);
            if(name == "Wall - Vertical") {
                setTile(mouseTile, new Wall(mouseTile));
                dynamic_cast<Wall*>(getTile(mouseTile))->isVertical = false;
            }
        } else {
            setTile(mouseTile, new Wall(mouseTile));
        }
    }

    if(isMiddleMouseClick && isTile(mouseTile)) {
        if(getTile(mouseTile) != nullptr) {
            /*std::string name = getTile(mouseTile)->name();
            destroyTile(mouseTile);
            if(name == "Track - Middle") {
                setTile(mouseTile, new Track(mouseTile));
                dynamic_cast<Track*>(getTile(mouseTile))->type = Track::END_LEFT;
            } else if(name == "Track - End Left") {
                setTile(mouseTile, new Track(mouseTile));
                dynamic_cast<Track*>(getTile(mouseTile))->type = Track::END_RIGHT;
                dynamic_cast<Track*>(getTile(mouseTile))->reversed = true;
            }*/
            Track* track = dynamic_cast<Track*>(getTile(mouseTile));
            track->type++;
            if(track->type == Track::TRACK_TYPE_COUNT) destroyTile(mouseTile);
        } else {
            setTile(mouseTile, new Track(mouseTile));
        }
    }

    float lineWidth = 1.5f;
    /*Texture2d::setColor(vec4(0, 0, 0, 0.5f));
    blank.draw(GRID_OFFSET.x, GRID_OFFSET.y, TILE_DIMS.x * GRID_DIMS.x, TILE_DIMS.y * GRID_DIMS.y);

    Texture2d::setColor(vec4(1.0f, 0.5f, 0.5f, 0.5f));
    //blank.draw(mx - 50, my - 50, 100.0f, 100.0f);

    for(int y = 0; y < GRID_DIMS.y + 1; y++) {
        blank.draw(GRID_OFFSET.x, GRID_OFFSET.y + TILE_DIMS.y * y, TILE_DIMS.x * GRID_DIMS.x, lineWidth);
    }
    for(int x = 0; x < GRID_DIMS.x + 1; x++) {
        blank.draw(GRID_OFFSET.x + TILE_DIMS.x * x, GRID_OFFSET.y, lineWidth, TILE_DIMS.y * GRID_DIMS.y);
    }*/

    Texture2d::setColor(vec4(0, 0, 0, 0.5f));
    for(int y = 0; y < GRID_DIMS.y; y++) {
        for(int x = 0; x < GRID_DIMS.x; x++) {
            blank.draw(GRID_OFFSET.x + TILE_DIMS.x * x + lineWidth, GRID_OFFSET.y + TILE_DIMS.y * y + lineWidth, TILE_DIMS.x - lineWidth, TILE_DIMS.y - lineWidth);
        }
    }



    for(int i = 0; i < items.size(); i++) {
        items[i]->update(deltaTime);
        items[i]->draw();
    }

    for(int i = 0; i < itemsToDestroy.size(); i++) {
        for(int j = 0; j < items.size(); j++) {
            if(itemsToDestroy[i] == items[j]) {
                items.erase(items.begin() + j);
                break;
            }
        }
    }
    itemsToDestroy.clear();

    for(int y = 0; y < GRID_DIMS.y; y++) {
        for(int x = 0; x < GRID_DIMS.x; x++) {
            Structure *tile = getTile(vec2(x, y));
            if(tile == nullptr) continue;
            tile->update(deltaTime);
            tile->draw();
        }
    }



    Texture2d::setColor(vec4(1, 1, 1, 0.5f));

    if(isTile(mouseTile)) {
        vec2 mouseTilePos = tilePos(mouseTile);
        blank.draw(mouseTilePos.x + lineWidth, mouseTilePos.y + lineWidth, TILE_DIMS.x - lineWidth, TILE_DIMS.y - lineWidth);
    }


    std::string text = std::to_string(score) + " | " + std::to_string(items.size());

    Texture2d::setColor(vec4(0, 0, 0, 0.5f));
    blank.draw(0, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 20, fontRenderer.getWidth(text) * fontScale + 20, 20 + fontRenderer.getHeight() * fontScale);

    fontRenderer.setColor(vec4(1, 0.5f, 0.5f, 1));
    fontRenderer.draw(text, 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);

    //tracksEndLeft.drawLoop(window->getTime(), GRID_OFFSET.x + 13, GRID_OFFSET.y + 24, TILE_DIMS.x, TILE_DIMS.y);
    //tracksMiddle.drawLoop(window->getTime(), GRID_OFFSET.x + 13 + TILE_DIMS.x, GRID_OFFSET.y + 24, TILE_DIMS.x, TILE_DIMS.y);
    //tracksEndRight.drawLoop(window->getTime(), GRID_OFFSET.x + 13 + TILE_DIMS.x * 2, GRID_OFFSET.y + 24, TILE_DIMS.x, TILE_DIMS.y);
}

void FactoryScene::cleanup() {
    for(int i = 0; i < items.size(); i++) delete items[i];
    items.clear();
    if(structs != nullptr) {
        for(int y = 0; y < GRID_DIMS.y; y++) {
            if(structs[y] != nullptr) {
                for(int x = 0; x < GRID_DIMS.x; x++) {
                    delete structs[y][x];
                    structs[y][x] = nullptr;
                }
            }
            delete [] structs[y];
            structs[y] = nullptr;
        }
        delete [] structs;
        structs = nullptr;
    }

}

void FactoryScene::keyPress(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_SPACE) {
            /*if(coins >= GUY_MURDER_COST) {
                coins -= GUY_MURDER_COST;
                float speed = 9999999.0f;
                int unit = -1;
                for(int i = 0; i < MAX_GUYS; i++) {
                    if(guys[i].exists) {
                        if(length(guys[i].getSpeed()) < speed) {
                            unit = i;
                            speed = length(guys[i].getSpeed());
                        }
                    }
                }
                if(unit != -1) guys[unit].exists = false;
            }*/
        } else if(key == GLFW_KEY_R) {
            cleanup();
            load();
        } else if(key == GLFW_KEY_F8) {
            debugMode = !debugMode;
        }
    }
}

void FactoryScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}

vec2 FactoryScene::getTileAtPos(vec2 pos) {
    int x = (pos.x - GRID_OFFSET.x) / TILE_DIMS.x;
    int y = (pos.y - GRID_OFFSET.y) / TILE_DIMS.y;
    return vec2(x, y);
}

vec2 FactoryScene::tilePos(vec2 tile) {
    return GRID_OFFSET + tile * TILE_DIMS;
}

Structure * FactoryScene::getTile(vec2 pos) {
    if(!isTile(pos)) return nullptr;
    return structs[static_cast<int>(pos.y)][static_cast<int>(pos.x)];
}

void FactoryScene::destroyTile(vec2 pos) {
    delete structs[static_cast<int>(pos.y)][static_cast<int>(pos.x)];
    structs[static_cast<int>(pos.y)][static_cast<int>(pos.x)] = nullptr;
}

void FactoryScene::setTile(vec2 pos, Structure *structure) {
    structs[static_cast<int>(pos.y)][static_cast<int>(pos.x)] = structure;
}

bool FactoryScene::isTile(vec2 tile) {
    return tile.x >= 0 && tile.y >= 0 && tile.x < GRID_DIMS.x && tile.y < GRID_DIMS.y;
}

bool FactoryScene::isTileFull(vec2 tile) {
    return structs[static_cast<int>(tile.y)][static_cast<int>(tile.x)] == nullptr;
}
