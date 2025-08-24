//
// Created by cobble on 6/10/2025.
//

#include "simulation.h"

#include "ew/ewMath/ewMath.h"


Camera SimulationScene::camera = Camera();
Window *SimulationScene::window = nullptr;
FontRenderer SimulationScene::fontRenderer = FontRenderer();
Texture2d SimulationScene::blank = Texture2d();
float SimulationScene::peak = 0.0f;
SimulationScene::Tile *SimulationScene::tiles = nullptr;
Team SimulationScene::green = Team(0, 0, vec3(0, 1, 0));
Team SimulationScene::red = Team(1, GRID_WIDTH * GRID_HEIGHT - 1, vec3(1, 0, 0));
Team SimulationScene::blue = Team(2, GRID_WIDTH * GRID_HEIGHT - GRID_WIDTH, vec3(0.25f, 0.5f, 1));
Team SimulationScene::purple = Team(3, GRID_WIDTH - 1, vec3(1, 0, 1));
Team *SimulationScene::teams[TEAM_COUNT] = {&green, &red, &blue, &purple};



SimulationScene::SimulationScene(Window *w) : Scene(w) {
    window = w;
}

SimulationScene::~SimulationScene() {
    cleanup();
}


void SimulationScene::load() {
    window->setWindowTitle("Simulation Thing");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");


    tiles = new Tile[GRID_WIDTH * GRID_HEIGHT];
    for(int y = 0; y < GRID_HEIGHT; y++) {
        for(int x = 0; x < GRID_WIDTH; x++) {
            tiles[y * GRID_WIDTH + x].type = ew::RandomRange(0, 1) > TREE_CHANCE ? GRASS : TREE;
            tiles[y * GRID_WIDTH + x].data = 0;
            tiles[y * GRID_WIDTH + x].height = -1;
        }
    }

    generateRiver(length(TILE_DIMS), length(TILE_DIMS) * 1.2f);
    generateHeights(-TILE_DIMS.x * 0.5f, TILE_DIMS.x * 0.5f, 0.0f, 10000.0f);


    //guys = new LittleGuy[MAX_GUYS];
    //
    //guys[0] = LittleGuy::createGuy(static_cast<int>(ew::RandomRange(0, GRID_WIDTH * GRID_HEIGHT)));

    std::vector<int> banks = std::vector<int>();

    for(int i = 0; i < BANK_COUNT; i++) {
        int bankTile = 0;
        int bankX = bankTile % GRID_WIDTH;
        int bankY = bankTile / GRID_WIDTH;
        bool valid = true;
        while((bankX < 2 || bankX >= GRID_WIDTH - 2 || bankY < 2 || bankY >= GRID_HEIGHT - 2 || tiles[bankTile].type != GRASS) || !valid) {
            bankTile = ew::RandomRange(0, GRID_WIDTH * GRID_HEIGHT);
            bankX = bankTile % GRID_WIDTH;
            bankY = bankTile / GRID_WIDTH;

            vec2 bankPos = vec2(bankX, bankY) * TILE_DIMS;
            valid = true;
            for(int j = 0; j < banks.size(); j++) {
                vec2 bank2Pos = vec2(banks[j] % GRID_WIDTH, banks[j] / GRID_WIDTH) * TILE_DIMS;
                if(distance(bankPos, bank2Pos) < MIN_BANK_DISTANCE) {
                    valid = false;
                    break;
                }
            }
        }

        for(int y = bankY - 2; y <= bankY + 2; y++) {
            for(int x = bankX - 2; x <= bankX + 2; x++) {
                if(tiles[y * GRID_WIDTH + x].type == TREE) {
                    tiles[y * GRID_WIDTH + x].type = GRASS;
                }
            }
        }
        tiles[bankTile].type = BANK;
        banks.push_back(bankTile);
    }



    for(int i = 0; i < GENERATOR_COUNT; i++) {
        int genTile = ew::RandomRange(0, GRID_WIDTH * GRID_HEIGHT);
        while(tiles[genTile].type != GRASS) genTile = ew::RandomRange(0, GRID_WIDTH * GRID_HEIGHT);
        tiles[genTile].type = GENERATOR;
    }

    /*tiles[1].type = GENERATOR;
    tiles[GRID_WIDTH].type = PATH;
    tiles[GRID_WIDTH + 1].type = PATH;

    tiles[GRID_WIDTH * GRID_HEIGHT - 2].type = GENERATOR;
    tiles[GRID_WIDTH * GRID_HEIGHT - GRID_WIDTH - 2].type = PATH;
    tiles[GRID_WIDTH * GRID_HEIGHT - GRID_WIDTH - 1].type = PATH;*/

    for(int i = 0; i < TEAM_COUNT; i++) teams[i]->load();

}

int lastTile = -1;
float lastTrigger = 0.0f;
float timePassed = 0.0f;
bool wasGDown = false;
bool overlayToggle = false;

void SimulationScene::draw() {
    float deltaTime = window->update();
    camera.update(window->window, deltaTime);
    mat4 viewProj = camera.proj * camera.view;
    float fontScale = 3.0f;

    if(glfwGetKey(window->window, GLFW_KEY_F)) {
        deltaTime *= 3;
        if(glfwGetKey(window->window, GLFW_KEY_LEFT_CONTROL)) {
            deltaTime *= 3;
        }
    }

    timePassed += deltaTime;

    bool leftMouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT);
    bool rightMouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_RIGHT);
    bool leftMousePressed = leftMouseDown && !wasMouseDown;
    bool gDown = glfwGetKey(window->window, GLFW_KEY_G);
    bool rightMousePressed = rightMouseDown && !wasRightMouseDown;
    bool leftMouseReleased = wasMouseDown && !leftMouseDown;
    bool rightMouseReleased = wasRightMouseDown && !rightMouseDown;
    bool gPressed = gDown && !wasGDown;
    wasMouseDown = leftMouseDown;
    wasRightMouseDown = rightMouseDown;
    wasGDown = gDown;

    drawBackground(true);

    Texture2d::gameCamera.reset();
    Texture2d::gameCamera.expandToInclude(0, 0);
    Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);

    if(gPressed) overlayToggle = !overlayToggle;


    if(timePassed - lastTrigger > 1.5f) { //triggers once per 1.5s
        lastTrigger = timePassed;
        for(int i=0;i<GRID_WIDTH*GRID_HEIGHT;i++) {
            if(tiles[i].type == GENERATOR) {
                tiles[i].data++;
            }
            if(tiles[i].type == SPAWNER) {
                Team *team = teams[tiles[i].data];
                //int spawnTile = i + GRID_WIDTH;
                //if(!validTile(spawnTile)) spawnTile = i - GRID_WIDTH;
                if(team->coins >= team->getGuyCost()) {
                    if(team->addGuy(team->spawnerTile)) {
                        team->coins -= Team::getGuyCost(team->guyCount - 1);
                    }
                }
            }
        }
    }

    float mx = window->mousePos.x;
    float my = window->mousePos.y;
    int mTile;
    if(mx >= 0 && my >= 0 && mx < GRID_WIDTH * TILE_DIMS.x && my < GRID_HEIGHT * TILE_DIMS.y) {
        int tileX = static_cast<int>(mx / TILE_DIMS.x);
        int tileY = static_cast<int>(my / TILE_DIMS.y);
        mTile = tileY * GRID_WIDTH + tileX;
        if(leftMousePressed || (leftMouseDown && mTile != lastTile)) {
            //if(leftMousePressed) std::cout << tileY * gridHeight + tileX << std::endl;
            //if(glfwGetKey(window->window, GLFW_KEY_LEFT_CONTROL)) {
                //tiles[mTile].type++;
                //if(tiles[tileY * GRID_WIDTH + tileX].type >= EVERY_TILE_COUNT) tiles[tileY * GRID_WIDTH + tileX].type = 0;
            //} else {
                //if(coins >= PATH_COST) {
                    if(tiles[mTile].type < BASIC_TILE_COUNT) {
                        tiles[mTile].type++;
                        if(tiles[tileY * GRID_WIDTH + tileX].type >= BASIC_TILE_COUNT) tiles[tileY * GRID_WIDTH + tileX].type = 0;
                        for(int i = 0; i < TEAM_COUNT; i++) teams[i]->getConnectedTiles();
                        //coins -= PATH_COST;
                    }
                //}

            //}

        }
        /*if(rightMouseDown) {
            for(int i = 0; i < MAX_GUYS; i++) {
                if(guyCount == MAX_GUYS && coins >= GUY_COST + GUY_MURDER_COST) {
                    coins -= GUY_MURDER_COST;
                    float speed = 9999999.0f;
                    int unit = -1;
                    for(int j = 0; j < MAX_GUYS; j++) {
                        if(guys[i].exists) {
                            if(length(guys[j].getSpeed()) < speed) {
                                unit = j;
                                speed = length(guys[j].getSpeed());
                            }
                        }
                    }

                    LittleGuy newGuy = LittleGuy::createGuy(mTile);
                    if(length(newGuy.getSpeed()) > speed) {
                        if(unit != -1) guys[unit].exists = false;
                        guys[unit] = newGuy;
                    }

                    coins -= GUY_COST;
                } else if(coins >= GUY_COST) {
                    if(guys[i].exists) continue;
                    guys[i] = LittleGuy::createGuy(mTile);
                    coins -= GUY_COST;
                }
                break;
            }
        }*/
        lastTile = mTile;
    }

    //draw tiles
    for(int y = 0; y < GRID_HEIGHT; y++) {
        for(int x = 0; x < GRID_WIDTH; x++) {
            vec3 col = vec3(0);
            float darkMult = 1.0f;
            int t = y * GRID_WIDTH + x;
            switch(tiles[t].type) {
                case PATH: col = vec3(204, 147, 86); break;
                case GRASS: col = vec3(104, 252, 143); break;
                case GENERATOR: col = vec3(150, 0, 150); break;
                case BANK: col = vec3(255, 184, 5); break;
                case WATER: col = vec3(100, 100, 255); break;
                case BRIDGE: col = vec3(117, 37, 0); darkMult = 0.5f; break;
                case TREE: col = vec3(3, 130, 37); darkMult = 0.5f; break;
                case SAND: col = vec3(255, 225, 175); break;
                case SPAWNER: {
                    col = teams[tiles[t].data]->color * 255.0f;
                    darkMult = 0.0f;
                    break;
                }
                default: col = vec3(0); break;
            }
            col /= 255.0f;
            float darkness = (peak - tiles[y * GRID_WIDTH + x].height) / peak * HEIGHT_STRENGTH * darkMult;
            col -= vec3(darkness);
            Texture2d::setColor(col);
            blank.draw(x * TILE_DIMS.x, y * TILE_DIMS.y, TILE_DIMS.x, TILE_DIMS.y);
        }
    }



    //tile overlays
    for(int y = 0; y < GRID_HEIGHT; y++) {
        for(int x = 0; x < GRID_WIDTH; x++) {
            bool text = true;
            switch(tiles[y * GRID_WIDTH + x].type) {
                case BANK: fontRenderer.setColor(vec4(0, 0, 0, 255) / 255.0f); break;
                case GENERATOR: fontRenderer.setColor(vec4(255, 184, 5, 255) / 255.0f); break;
                case GRASS:
                case WATER: if(debugMode) fontRenderer.setColor(vec4(255, 255, 255, 255) / 255.0f); else text = false; break;
                default: text = false; break;
            }
            if(text) {
                float coinFontScale = fontScale;
                //if(debugMode && (tiles[y * GRID_HEIGHT + x].type == WATER || tiles[y * GRID_HEIGHT + x].type == GRASS)) tiles[y * GRID_HEIGHT + x].data = tiles[y * GRID_HEIGHT + x].height;
                std::string coinStr = (tiles[y * GRID_WIDTH + x].type == GRASS || tiles[y * GRID_WIDTH + x].type == WATER) ? std::to_string(static_cast<int>(tiles[y * GRID_WIDTH + x].height)) : std::to_string(tiles[y * GRID_WIDTH + x].data);
                for(int i = 1; i < coinStr.length();i++) coinFontScale *= 0.75f;
                fontRenderer.draw(coinStr, x * TILE_DIMS.x + (TILE_DIMS.x * 0.5f) - (fontRenderer.getWidth(coinStr) * coinFontScale * 0.5f), y * TILE_DIMS.y + (TILE_DIMS.y * 0.5f - (fontRenderer.getHeight() * coinFontScale * 0.5f)), coinFontScale);
            }
        }
    }

    for(int i = 0; i < TEAM_COUNT; i++) {
        if(overlayToggle) {
            if(teams[i]->getScore() == 0.0f) continue;
            for(int j = 0; j < teams[i]->connectedPaths.size();j++) {
                Texture2d::setColor(vec4(teams[i]->color, 0.2f));
                int t = teams[i]->connectedPaths[j];
                blank.draw((t % GRID_WIDTH) * TILE_DIMS.x, (t / GRID_WIDTH) * TILE_DIMS.y, TILE_DIMS.x, TILE_DIMS.y);
            }
            for(int j = 0; j < teams[i]->teamGoalTileList.size(); j++) {
                Texture2d::setColor(vec4(teams[i]->color, 0.6f));
                int t = teams[i]->teamGoalTileList[j];
                blank.draw((t % GRID_WIDTH) * TILE_DIMS.x, (t / GRID_WIDTH) * TILE_DIMS.y, TILE_DIMS.x, TILE_DIMS.y);
            }
        }
        teams[i]->update(deltaTime);
        teams[i]->draw();
    }

    //fontRenderer.setColor(vec4(255, 184, 5, 255) / 255.0f);
    /*fontRenderer.setColor(green.color);
    fontRenderer.draw("Green Coins: " + std::to_string(green.coins) + "\nGuy Count: " + std::to_string(green.guyCount) + "/" + std::to_string(MAX_GUYS), 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);

    fontRenderer.setColor(red.color);
    std::string redText = "Red Coins: " + std::to_string(red.coins) + "\nGuy Count: " + std::to_string(red.guyCount) + "/" + std::to_string(MAX_GUYS);
    fontRenderer.draw(redText, Window::GAME_WIDTH - 10 - fontRenderer.getWidth(redText) * fontScale, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);*/

    bool coinsShown = static_cast<int>(window->getTime() / 5.0f) % 2 == 0;

    if(coinsShown) {
        fontRenderer.setColor(blue.color);
        fontRenderer.draw("Blue Coins: " + std::to_string(blue.coins), 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);
        fontRenderer.setColor(green.color);
        fontRenderer.draw("Green Coins: " + std::to_string(green.coins), 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale * 2 - 25, fontScale);
        fontRenderer.setColor(red.color);
        std::string redText = "Red Coins: " + std::to_string(red.coins);
        fontRenderer.draw(redText, Window::GAME_WIDTH - 10 - fontRenderer.getWidth(redText) * fontScale, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);
        fontRenderer.setColor(purple.color);
        std::string purpleText = "Purple Coins: " + std::to_string(purple.coins);
        fontRenderer.draw(purpleText, Window::GAME_WIDTH - 10 - fontRenderer.getWidth(purpleText) * fontScale, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale * 2 - 25, fontScale);
    } else {
        fontRenderer.setColor(blue.color);
        fontRenderer.draw("Blue Guys: " + std::to_string(blue.guyCount) + "/" + std::to_string(MAX_GUYS), 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);
        fontRenderer.setColor(green.color);
        fontRenderer.draw("Green Guys: " + std::to_string(green.guyCount) + "/" + std::to_string(MAX_GUYS), 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale * 2 - 25, fontScale);
        fontRenderer.setColor(red.color);
        std::string redText = "Red Guys: " + std::to_string(red.guyCount) + "/" + std::to_string(MAX_GUYS);
        fontRenderer.draw(redText, Window::GAME_WIDTH - 10 - fontRenderer.getWidth(redText) * fontScale, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);
        fontRenderer.setColor(purple.color);
        std::string purpleText = "Purple Guys: " + std::to_string(purple.guyCount) + "/" + std::to_string(MAX_GUYS);
        fontRenderer.draw(purpleText, Window::GAME_WIDTH - 10 - fontRenderer.getWidth(purpleText) * fontScale, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale * 2 - 25, fontScale);
    }


    float barWidth = 500.0f;
    float barHeight = 50.0f;
    float barPadding = 3.0f;
    float totalScore = 0.0f;//green.getScore() + red.getScore();
    for(int i = 0; i < TEAM_COUNT; i++) totalScore += teams[i]->getScore();
    Texture2d::setColor(vec3(1));
    blank.draw(Window::GAME_WIDTH * 0.5f - barWidth * 0.5f, (Window::GAME_HEIGHT + GRID_HEIGHT * TILE_DIMS.y) * 0.5f - barHeight * 0.5f, barWidth, barHeight, true);
    float offset = 0.0f;

    for(int i = 0; i < TEAM_COUNT; i++) {
        float percent = teams[i]->getScore() / totalScore;
        Texture2d::setColor(teams[i]->color);
        float w = (barWidth - barPadding * 2.0f) * percent;
        blank.draw(Window::GAME_WIDTH * 0.5f - barWidth * 0.5f + offset + barPadding, (Window::GAME_HEIGHT + GRID_HEIGHT * TILE_DIMS.y) * 0.5f - barHeight * 0.5f + barPadding, w, barHeight - barPadding * 2.0f, true);
        offset += w;
        if(percent == 1.0f) {
            std::cout << "Team: " << teams[i]->index << " has won!" << std::endl;
            cleanup();
            load();
        }
    }

    /*Texture2d::setColor(red.color);
    blank.draw(GRID_WIDTH * TILE_DIMS.x * 0.5f - barWidth * 0.5f + barPadding, (Window::GAME_HEIGHT + GRID_HEIGHT * TILE_DIMS.y) * 0.5f - barHeight * 0.5f + barPadding, barWidth - barPadding * 2.0f, barHeight - barPadding * 2.0f, true);
    Texture2d::setColor(green.color);
    blank.draw(GRID_WIDTH * TILE_DIMS.x * 0.5f - barWidth * 0.5f + barPadding, (Window::GAME_HEIGHT + GRID_HEIGHT * TILE_DIMS.y) * 0.5f - barHeight * 0.5f + barPadding, (barWidth - barPadding * 2.0f) * green.getScore() / totalScore, barHeight - barPadding * 2.0f, true);

    if(red.guyCount == 0 && red.coins < GUY_COST[0]) {
        std::cout << "Green has won!" << std::endl;
        cleanup();
        load();
    } else if(green.guyCount == 0 && green.coins < GUY_COST[0]) {
        std::cout << "Red has won!" << std::endl;
        cleanup();
        load();
    }*/

}

void SimulationScene::cleanup() {
    delete [] tiles;
    green.cleanup();
    red.cleanup();
}

void SimulationScene::drawBackground(bool pride) {
    float visibility = 0.05f * 3;
    vec4 blue = vec4(91, 206, 250, visibility * 255.0f) / 255.0f;
    vec4 pink = vec4(245, 169, 184, visibility * 255.0f) / 255.0f;
    vec4 white = vec4(255, 255, 255, visibility * 255.0f) / 255.0f;

    float x = window->gx;
    float y = window->gy;
    float w = window->gw;
    float h = window->gh;

    Texture2d::setColor(vec4(0, 0, 0, 1));
    blank.drawRaw(x, y, w, h, true);

    if(pride) {
        Texture2d::setColor(blue);
        blank.drawRaw(x, y + (h * 0.0f), w, h * 0.2f);
        Texture2d::setColor(pink);
        blank.drawRaw(x, y + (h * 0.2f), w, h * 0.2f);
        Texture2d::setColor(white);
        blank.drawRaw(x, y + (h * 0.4f), w, h * 0.2f);
        Texture2d::setColor(pink);
        blank.drawRaw(x, y + (h * 0.6f), w, h * 0.2f);
        Texture2d::setColor(blue);
        blank.drawRaw(x, y + (h * 0.8f), w, h * 0.2f);
    }

}

void SimulationScene::keyPress(int key, int action, int mods) {
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

void SimulationScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}

bool SimulationScene::isPath(int tile) {
    if(tile < 0 || tile >= GRID_WIDTH * GRID_HEIGHT) return false;
    return tiles[tile].type == PATH || tiles[tile].type == BRIDGE;
}

bool SimulationScene::wraps(int tile1, int tile2) {
    if(tile1 % GRID_WIDTH == 0 && tile2 % GRID_WIDTH == GRID_WIDTH - 1) return true;
    if(tile2 % GRID_WIDTH == 0 && tile1 % GRID_WIDTH == GRID_WIDTH - 1) return true;
    return false;
}

bool SimulationScene::validTile(int tile) {
    return !(tile < 0 || tile >= GRID_WIDTH * GRID_HEIGHT);
}

void SimulationScene::generateRiver(float riverWidth, float bridgeWidth) {

    vec2 center = vec2(GRID_WIDTH * TILE_DIMS.x * 0.5f, GRID_HEIGHT * TILE_DIMS.y * 0.5f);
    vec2 edgePoint = vec2(0);
    bool isTooClose = true;
    while(isTooClose) {
        float angle = ew::RandomRange(0, ew::PI * 2.0f);
        vec2 halfSize = vec2(GRID_WIDTH, GRID_HEIGHT) * TILE_DIMS * 0.5f;
        vec2 dir = vec2(cos(angle), sin(angle));
        float scaleX = halfSize.x / std::abs(dir.x);
        float scaleY = halfSize.y / std::abs(dir.y);
        float scale = std::min(scaleX, scaleY);
        edgePoint = center + (dir * scale);
        isTooClose = false;
        for(int i = 0; i < TEAM_COUNT; i++) {
            vec2 teamSpawn = vec2(teams[i]->spawnerTile % GRID_WIDTH, teams[i]->spawnerTile / GRID_WIDTH) * TILE_DIMS + TILE_DIMS * 0.5f;
            if(distance(edgePoint, teamSpawn) < riverWidth * 5) {
                isTooClose = true;
                break;
            }
        }
    }

    float distPerSegment = 7.5f;
    float maxAngleChange = 6.0f;
    float bridgeEveryUnits = 500.0f;

    std::vector<vec2> riverPoints = std::vector<vec2>();
    riverPoints.push_back(edgePoint);
    riverPoints.push_back(edgePoint + normalize(center - edgePoint) * distPerSegment);
    std::cout << "River:" << std::endl;
    std::cout << " - Start: " << edgePoint.x << ", " << edgePoint.y << std::endl;

    while(riverPoints[riverPoints.size() - 1].x >= 0 && riverPoints[riverPoints.size() - 1].x < GRID_WIDTH * TILE_DIMS.x && riverPoints[riverPoints.size() - 1].y >= 0 && riverPoints[riverPoints.size() - 1].y < GRID_HEIGHT * TILE_DIMS.y) {
        vec2 beforePrev = riverPoints[riverPoints.size() - 2];
        vec2 prev = riverPoints[riverPoints.size() - 1];
        vec2 dirChange = normalize(prev - beforePrev);

        float angleOffset = ew::RandomRange(-maxAngleChange, maxAngleChange);

        vec2 newDir = rotate(dirChange, angleOffset * ew::DEG2RAD);
        riverPoints.push_back(prev + newDir * distPerSegment);
    }

    std::cout << " - End: " << riverPoints[riverPoints.size() - 1].x << ", " << riverPoints[riverPoints.size() - 1].y << std::endl;


    for(int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
        vec2 tilePoint = vec2((i % GRID_WIDTH) * TILE_DIMS.x, (i / GRID_WIDTH) * TILE_DIMS.y);
        for(int j = 1; j < riverPoints.size(); j++) {
            vec2 A = riverPoints[j];
            vec2 B = riverPoints[j - 1];
            vec2 AP = tilePoint - A;
            vec2 AB = B - A;
            float abLengthSq = dot(AB, AB);
            float t = dot(AP, AB) / abLengthSq;
            t = clamp(t, 0.0f, 1.0f);
            vec2 closest = A + t * AB;
            float distToClosestPoint = length(tilePoint - closest);
            if(distToClosestPoint < riverWidth) {
                tiles[i].type = WATER;
                if(tiles[i].height == -1 || distToClosestPoint < tiles[i].height) tiles[i].height = distToClosestPoint;
            }
        }
    }


    //generate bridges
    std::vector<vec2> bridgePoints = std::vector<vec2>();
    float distSinceBridge = 0.0f;
    for(int i = 1; i < riverPoints.size(); i++) {
        float distForBridge = bridgePoints.size() == 0 ? bridgeEveryUnits * 0.5f : bridgeEveryUnits;
        distSinceBridge += length(riverPoints[i] - riverPoints[i - 1]);
        if(distSinceBridge >= distForBridge) {
            bridgePoints.push_back(riverPoints[i]);
            distSinceBridge = 0.0f;
        }
    }

    std::cout << " - Bridges: " << bridgePoints.size() << std::endl;
    for(int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
        vec2 tilePoint = vec2((i % GRID_WIDTH) * TILE_DIMS.x, (i / GRID_WIDTH) * TILE_DIMS.y);
        for(int j = 0; j < bridgePoints.size(); j++) {
            float distToClosestPoint = length(tilePoint - bridgePoints[j]);
            if(distToClosestPoint < bridgeWidth) {
                tiles[i].type = BRIDGE;
                tiles[i].height = -1.0f;
                //if(tiles[i].height == -1 || distToClosestPoint < tiles[i].height) tiles[i].height = bridgeWidth - distToClosestPoint;
            }
        }
    }

}

float SimulationScene::getSurroundingHeights(int tile) {
    if(!validTile(tile)) return -1.0f;
    float avg = 0.0f;
    int validTiles = 0;

    if(validTile(tile + 1) && !wraps(tile, tile + 1) && tiles[tile + 1].height != -1.0f) {
        avg += tiles[tile + 1].height;
        validTiles++;
    }
    if(validTile(tile - 1) && !wraps(tile, tile - 1) && tiles[tile - 1].height != -1.0f) {
        avg += tiles[tile - 1].height;
        validTiles++;
    }
    if(validTile(tile + GRID_WIDTH) && tiles[tile + GRID_WIDTH].height != -1.0f) {
        avg += tiles[tile + GRID_WIDTH].height;
        validTiles++;
    }
    if(validTile(tile - GRID_WIDTH) && tiles[tile - GRID_WIDTH].height != -1.0f) {
        avg += tiles[tile - GRID_WIDTH].height;
        validTiles++;
    }


    if(validTiles == 0) return -1.0f;
    return avg / static_cast<float>(validTiles);
}

bool SimulationScene::isNextToWater(int tile) {
    if(validTile(tile + 1) && !wraps(tile, tile + 1) && tiles[tile + 1].type == WATER) return true;
    if(validTile(tile + 1 - GRID_WIDTH) && !wraps(tile, tile + 1)  && tiles[tile + 1 - GRID_WIDTH].type == WATER) return true;
    if(validTile(tile - GRID_WIDTH) && tiles[tile - GRID_WIDTH].type == WATER) return true;
    if(validTile(tile - 1 - GRID_WIDTH) && !wraps(tile, tile - 1)  && tiles[tile - 1 - GRID_WIDTH].type == WATER) return true;
    if(validTile(tile - 1) && !wraps(tile, tile - 1)  && tiles[tile - 1].type == WATER) return true;
    if(validTile(tile - 1 + GRID_WIDTH) && !wraps(tile, tile - 1)  && tiles[tile - 1 + GRID_WIDTH].type == WATER) return true;
    if(validTile(tile + GRID_WIDTH) && tiles[tile + GRID_WIDTH].type == WATER) return true;
    if(validTile(tile + 1 + GRID_WIDTH) && !wraps(tile, tile + 1)  && tiles[tile + 1 + GRID_WIDTH].type == WATER) return true;


    return false;
}

void SimulationScene::generateHeights(float heightVaryMin, float heightVaryMax, float minHeight, float maxHeight) {
    peak = 0.0f;
    while(true) {
        bool done = true;
        for(int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
            if(tiles[i].height != -1.0f) continue;
            float height = getSurroundingHeights(i);
            if(height != -1.0f) {
                tiles[i].height = clamp(height + ew::RandomRange(heightVaryMin, heightVaryMax), minHeight, maxHeight);
                peak = max(peak, tiles[i].height);
            }

            done = false;
        }
        if(done) break;
    }

}



