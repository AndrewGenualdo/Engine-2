//
// Created by cobble on 8/19/2025.
//

#include "world.h"






Camera WorldScene::camera = Camera();
Window *WorldScene::window = nullptr;
FontRenderer WorldScene::fontRenderer = FontRenderer();
bool WorldScene::debugMode = false;
std::map<int, Texture2d> WorldScene::blocks = std::map<int, Texture2d>();
std::map<int, int*> WorldScene::blockVariants = std::map<int, int*>();
Texture2d WorldScene::blank = Texture2d();
Block ***WorldScene::world = nullptr;
vec2 WorldScene::screenOffset = vec2(500);
vec2 WorldScene::tempScreenOffset = vec2(0);
std::map<int, RailTypeData> WorldScene::railPaths = std::map<int, RailTypeData>();

WorldScene::WorldScene(Window *w) {
    window = w;
}

WorldScene::~WorldScene() {
    cleanup();
}

void WorldScene::load() {
    window->setWindowTitle("World");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");

    blocks.clear();
    blocks[TEMPLATE] = Texture2d("assets/textures/blocks/template.png");
    blocks[AIR] = Texture2d("assets/textures/blocks/air.png");
    blocks[GRASS] = Texture2d("assets/textures/blocks/grass.png");
    blocks[DIRT] = Texture2d("assets/textures/blocks/dirt.png");
    blocks[STONE] = Texture2d("assets/textures/blocks/stone.png");
    blocks[LOG] = Texture2d("assets/textures/blocks/log.png");



    for(int i = 0; i < BELT; i++) blockVariants[i] = new int[]{0};

    blocks[BELT] = Texture2d("assets/textures/blocks/belt/default.png");
    blocks[BELT_OFFSET + 00] = Texture2d("assets/textures/blocks/belt/default.png");
    blocks[BELT_OFFSET + 01] = Texture2d("assets/textures/blocks/belt/default_to_north.png");
    blocks[BELT_OFFSET + 02] = Texture2d("assets/textures/blocks/belt/default_to_east.png");
    blocks[BELT_OFFSET + 03] = Texture2d("assets/textures/blocks/belt/default_to_south.png");
    blocks[BELT_OFFSET + 04] = Texture2d("assets/textures/blocks/belt/default_to_west.png");
    blocks[BELT_OFFSET + 10] = Texture2d("assets/textures/blocks/belt/north_to_default.png");
    blocks[BELT_OFFSET + 12] = Texture2d("assets/textures/blocks/belt/north_to_east.png");
    blocks[BELT_OFFSET + 13] = Texture2d("assets/textures/blocks/belt/north_to_south.png");
    blocks[BELT_OFFSET + 14] = Texture2d("assets/textures/blocks/belt/north_to_west.png");
    blocks[BELT_OFFSET + 20] = Texture2d("assets/textures/blocks/belt/east_to_default.png");
    blocks[BELT_OFFSET + 21] = Texture2d("assets/textures/blocks/belt/east_to_north.png");
    blocks[BELT_OFFSET + 23] = Texture2d("assets/textures/blocks/belt/east_to_south.png");
    blocks[BELT_OFFSET + 24] = Texture2d("assets/textures/blocks/belt/east_to_west.png");
    blocks[BELT_OFFSET + 30] = Texture2d("assets/textures/blocks/belt/south_to_default.png");
    blocks[BELT_OFFSET + 31] = Texture2d("assets/textures/blocks/belt/south_to_north.png");
    blocks[BELT_OFFSET + 32] = Texture2d("assets/textures/blocks/belt/south_to_east.png");
    blocks[BELT_OFFSET + 34] = Texture2d("assets/textures/blocks/belt/south_to_west.png");
    blocks[BELT_OFFSET + 40] = Texture2d("assets/textures/blocks/belt/west_to_default.png");
    blocks[BELT_OFFSET + 41] = Texture2d("assets/textures/blocks/belt/west_to_north.png");
    blocks[BELT_OFFSET + 42] = Texture2d("assets/textures/blocks/belt/west_to_east.png");
    blocks[BELT_OFFSET + 43] = Texture2d("assets/textures/blocks/belt/west_to_south.png");
    blockVariants[BELT] = new int[]{00, 01, 02, 03, 04, 10, 12, 13, 14, 20, 21, 23, 24, 30, 31, 32, 34, 40, 41, 42, 43};


    blocks[RAIL] = Texture2d("assets/textures/blocks/rail/" + std::to_string(NORTH * SOUTH) + ".png");
    blocks[RAIL_OFFSET + NORTH * NORTH] = Texture2d("assets/textures/blocks/rail/" + std::to_string(NORTH * NORTH) + ".png"); //1*
    blocks[RAIL_OFFSET + NORTH * EAST] = Texture2d("assets/textures/blocks/rail/" + std::to_string(NORTH * EAST) + ".png"); //2*
    blocks[RAIL_OFFSET + NORTH * SOUTH] = Texture2d("assets/textures/blocks/rail/" + std::to_string(NORTH * SOUTH) + ".png"); //6*
    blocks[RAIL_OFFSET + NORTH * WEST] = Texture2d("assets/textures/blocks/rail/" + std::to_string(NORTH * WEST) + ".png"); //24*
    blocks[RAIL_OFFSET + EAST * EAST] = Texture2d("assets/textures/blocks/rail/" + std::to_string(EAST * EAST) + ".png"); //4*
    blocks[RAIL_OFFSET + EAST * SOUTH] = Texture2d("assets/textures/blocks/rail/" + std::to_string(EAST * SOUTH) + ".png"); //12*
    blocks[RAIL_OFFSET + EAST * WEST] = Texture2d("assets/textures/blocks/rail/" + std::to_string(EAST * WEST) + ".png"); //48*
    blocks[RAIL_OFFSET + SOUTH * SOUTH] = Texture2d("assets/textures/blocks/rail/" + std::to_string(SOUTH * SOUTH) + ".png"); //36*
    blocks[RAIL_OFFSET + SOUTH * WEST] = Texture2d("assets/textures/blocks/rail/" + std::to_string(SOUTH * WEST) + ".png"); //144*
    blocks[RAIL_OFFSET + WEST * WEST] = Texture2d("assets/textures/blocks/rail/" + std::to_string(WEST * WEST) + ".png"); //576*
    blocks[RAIL_OFFSET + NORTH * SOUTH + UP_FIRST] = Texture2d("assets/textures/blocks/rail/" + std::to_string(NORTH * SOUTH + UP_FIRST) + ".png"); //7*
    blocks[RAIL_OFFSET + NORTH * SOUTH + UP_SECOND] = Texture2d("assets/textures/blocks/rail/" + std::to_string(NORTH * SOUTH + UP_SECOND) + ".png"); //9*
    blocks[RAIL_OFFSET + EAST * WEST + UP_FIRST] = Texture2d("assets/textures/blocks/rail/" + std::to_string(EAST * WEST + UP_FIRST) + ".png"); //49*
    blocks[RAIL_OFFSET + EAST * WEST + UP_SECOND] = Texture2d("assets/textures/blocks/rail/" + std::to_string(EAST * WEST + UP_SECOND) + ".png");//51*
    blockVariants[RAIL] = new int[]{1, 2, 6, 24, 4, 12, 48, 36, 144, 576, 7, 9, 49, 51};

    blocks[CART_OFFSET + NORTH * NORTH] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(NORTH * SOUTH) + ".png");
    blocks[CART_OFFSET + NORTH * EAST] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(NORTH * EAST) + ".png"); //2*
    blocks[CART_OFFSET + NORTH * SOUTH] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(NORTH * SOUTH) + ".png");
    blocks[CART_OFFSET + NORTH * WEST] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(NORTH * WEST) + ".png"); //24*
    blocks[CART_OFFSET + EAST * EAST] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(EAST * WEST) + ".png");
    blocks[CART_OFFSET + EAST * SOUTH] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(EAST * SOUTH) + ".png"); //12*
    blocks[CART_OFFSET + EAST * WEST] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(EAST * WEST) + ".png");
    blocks[CART_OFFSET + SOUTH * SOUTH] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(NORTH * SOUTH) + ".png");
    blocks[CART_OFFSET + SOUTH * WEST] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(SOUTH * WEST) + ".png"); //144*
    blocks[CART_OFFSET + WEST * WEST] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(EAST * WEST) + ".png");
    blocks[CART_OFFSET + NORTH * SOUTH + UP_FIRST] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(NORTH * SOUTH + UP_FIRST) + ".png"); //7*
    blocks[CART_OFFSET + NORTH * SOUTH + UP_SECOND] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(NORTH * SOUTH + UP_SECOND) + ".png"); //9*
    blocks[CART_OFFSET + EAST * WEST + UP_FIRST] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(EAST * WEST + UP_FIRST) + ".png"); //49*
    blocks[CART_OFFSET + EAST * WEST + UP_SECOND] = Texture2d("assets/textures/blocks/rail/cart_" + std::to_string(EAST * WEST + UP_SECOND) + ".png");//51*

    railPaths.clear();
    railPaths[NORTH * NORTH] = RailTypeData(ivec3(-1, 0, 0), ivec3(-1, 0, 0), {vec2(-0.25f, 0.125f), vec2(0.0f, 0.0f), vec2(-0.25f, 0.125f)});
    railPaths[NORTH * EAST] = RailTypeData(ivec3(-1, 0, 0), ivec3(0, 0, -1), {vec2(-0.25f, 0.125f), vec2(0.0f, 0.0f), vec2(0.25f, 0.125f)});
    railPaths[NORTH * SOUTH] = RailTypeData(ivec3(-1, 0, 0), ivec3(1, 0, 0), {vec2(-0.25f, 0.125f), vec2(0.25f, -0.125f)});
    railPaths[NORTH * WEST] = RailTypeData(ivec3(-1, 0, 0), ivec3(0, 0, 1), {vec2(-0.25f, 0.125f), vec2(0.0f, 0.0f), vec2(-0.25f, -0.125f)});
    railPaths[EAST * EAST] = RailTypeData(ivec3(0, 0, -1), ivec3(0, 0, -1), {vec2(0.25f, 0.125f), vec2(0.0f, 0.0f), vec2(0.25f, 0.125f)});
    railPaths[EAST * SOUTH] = RailTypeData(ivec3(0, 0, -1), ivec3(1, 0, 0), {vec2(0.25f, 0.125f), vec2(0.0f, 0.0f), vec2(0.25f, -0.125f)});
    railPaths[EAST * WEST] = RailTypeData(ivec3(0, 0, -1), ivec3(0, 0, 1), {vec2(0.25f, 0.125f), vec2(-0.25f, -0.125f)});
    railPaths[SOUTH * SOUTH] = RailTypeData(ivec3(1, 0, 0), ivec3(1, 0, 0), {vec2(0.25f, -0.125f), vec2(0.0f, 0.0f), vec2(0.25f, -0.125f)});
    railPaths[SOUTH * WEST] = RailTypeData(ivec3(1, 0, 0), ivec3(0, 0, 1), {vec2(0.25f, -0.125f), vec2(0.0f, 0.0f), vec2(-0.25f, -0.125f)});
    railPaths[WEST * WEST] = RailTypeData(ivec3(0, 0, 1), ivec3(0, 0, 1), {vec2(-0.25f, -0.125f), vec2(0.0f, 0.0f), vec2(-0.25f, -0.125f)});
    railPaths[NORTH * SOUTH + UP_FIRST] = RailTypeData(ivec3(-1, 1, 0), ivec3(1, 0, 0), {vec2(-0.25f, 0.125f + 0.5f), vec2(0.25f, -0.125f)});
    railPaths[NORTH * SOUTH + UP_SECOND] = RailTypeData(ivec3(1, 1, 0), ivec3(-1, 0, 0), {vec2(0.25f, -0.125f + 0.5f), vec2(-0.25f, 0.125f)});
    railPaths[EAST * WEST + UP_FIRST] = RailTypeData(ivec3(0, 1, -1), ivec3(0, 0, 1), {vec2(0.25f, 0.125f + 0.5f), vec2(-0.25f, -0.125f)});
    railPaths[EAST * WEST + UP_SECOND] = RailTypeData(ivec3(0, 1, 1), ivec3(0, 0, -1), {vec2(-0.25f, -0.125f + 0.5f), vec2(0.25f, 0.125f)});

    if(world == nullptr) {
        world = new Block**[WORLD_SIZE];
        for(int i = 0; i < WORLD_SIZE; i++) {
            world[i] = new Block*[WORLD_SIZE];
            for(int j = 0; j < WORLD_SIZE; j++) {
                world[i][j] = new Block[WORLD_SIZE];
            }
        }
        for(int y = 0; y < WORLD_SIZE; y++) {
            for(int x = 0; x < WORLD_SIZE; x++) {
                for(int z = 0; z < WORLD_SIZE; z++) {
                    world[y][x][z] = Block();

                    if(y == WORLD_SIZE - 3 && x == 0) world[y][x][z].type = LOG;
                    else if(y > WORLD_SIZE - 4) world[y][x][z].type = AIR;
                    /*if(y == WORLD_SIZE - 3 && x == WORLD_SIZE - 1) {
                        world[y][x][z].type = BELT;
                        if(z == WORLD_SIZE - 1) world[y][x][z].data = 20;
                        else if(z == 0) world[y][x][z].data = 04;
                        else world[y][x][z].data = 24;
                    }
                    else if(y == WORLD_SIZE - 3 && x == WORLD_SIZE - 3) {
                        world[y][x][z].type = BELT;
                        if(z == WORLD_SIZE - 1) world[y][x][z].data = 02;
                        else if(z == 0) world[y][x][z].data = 40;
                        else world[y][x][z].data = 42;
                    }*/
                    else if(y == WORLD_SIZE - 4) world[y][x][z].type = GRASS;
                    else if(y < WORLD_SIZE - 4 && y > WORLD_SIZE - ew::RandomRange(5, 9)) world[y][x][z].type = DIRT;
                    else if(y < WORLD_SIZE - 4) world[y][x][z].type = STONE;
                }
            }
        }
    }


}

void WorldScene::draw() {
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
    //block.draw(500, 500, 100, 100.0f); //z = x + 50, y + 25   |   x = x - 50, y + 25   |   y = y + 50
    //block.draw((int) mx, (int) my, 100.0f, 100.0f);

    if(window->isInputClicked(GLFW_MOUSE_BUTTON_MIDDLE)) mouseStart = vec2(mx, my);
    if(window->isInputPressed(GLFW_MOUSE_BUTTON_MIDDLE)) tempScreenOffset = vec2(mx, my) - mouseStart;
    else tempScreenOffset = vec2(0);
    if(window->isInputReleased(GLFW_MOUSE_BUTTON_MIDDLE)) screenOffset += vec2(mx, my) - mouseStart;

    ivec3 hoverBlock = vec3(-1);


    for(int y = 0; y < WORLD_SIZE; y++) {
        for(int x = 0; x < WORLD_SIZE; x++) {
            for(int z = 0; z < WORLD_SIZE; z++) {

                if((WORLD_SIZE - 1) - x + (WORLD_SIZE - 1) - z < zoom - 1) continue;
                //else if((WORLD_SIZE - 1) - x + (WORLD_SIZE - 1) - z < zoom) Texture2d::setColor(vec4(vec3(1), 0.2f));
                else Texture2d::setColor(vec4(1));

                vec3 wPos = vec3(x, y, z) + 0.5f;
                vec2 pos = vec2( wPos.z * -50 + wPos.x * 50, wPos.z * -25 + wPos.x * -25 + wPos.y * 50) + screenOffset + tempScreenOffset;
                if(pos.x < 0 || pos.y < 0 || pos.x > Window::GAME_WIDTH || pos.y > Window::GAME_HEIGHT) continue;

                if(world[y][x][z].type != AIR && mx > pos.x && my > pos.y && mx < pos.x + 100 && my < pos.y + 100) hoverBlock = ivec3(x, y, z);
                world[y][x][z].draw(pos.x, pos.y, 100.0f, 100.0f);
            }
        }
    }
    cart.update(deltaTime);
    cart.draw(100.0f, 100.0f);

    if(hoverBlock != ivec3(-1)) {
        Block* block = getBlock(hoverBlock);
        vec3 worldPos = vec3(hoverBlock) + 0.5f;
        vec2 pos = vec2(worldPos.z * -50 + worldPos.x * 50, worldPos.z * -25 + worldPos.x * -25 + worldPos.y * 50) + screenOffset + tempScreenOffset;
        Texture2d::setColor(vec4(vec3(1), 0.4f));
        blocks[TEMPLATE].draw(pos.x, pos.y, 100.0f, 100.0f);
        Texture2d::setColor(vec4(1));
        fontRenderer.setColor(vec4(1));
        fontRenderer.draw(std::to_string(block->type) + ":" + std::to_string(block->data) + "\n" + std::to_string(static_cast<int>(hoverBlock.x)) + ", " + std::to_string(static_cast<int>(hoverBlock.y)) + ", " + std::to_string(static_cast<int>(hoverBlock.z)), 10, Window::GAME_HEIGHT - 10 - fontRenderer.getHeight() * fontScale, fontScale);

        if(beltStart != ivec3(-1)) {
            vec3 startWorldPos = vec3(beltStart) + 0.5f;
            vec2 startPos = vec2(startWorldPos.z * -50 + startWorldPos.x * 50, startWorldPos.z * -25 + startWorldPos.x * -25 + startWorldPos.y * 50) + screenOffset + tempScreenOffset;
            Texture2d::setColor(vec4(0.5f, 1.0f, 0.5f, 0.4f));
            blocks[TEMPLATE].draw(startPos.x, startPos.y, 100.0f, 100.0f);
            Texture2d::setColor(vec4(1));
        }
    }

    if(window->isInputClicked(GLFW_MOUSE_BUTTON_RIGHT)) beltStart = hoverBlock;
    else if(window->isInputPressed(GLFW_MOUSE_BUTTON_RIGHT)) {

    } else if(window->isInputReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
        ivec3 beltEnd = hoverBlock;

        ivec3 dir = vec3(ew::normalize(beltEnd.x - beltStart.x), ew::normalize(beltEnd.y - beltStart.y), ew::normalize(beltEnd.z - beltStart.z));

        ivec3 pos = beltStart;
        ivec3 amt = beltEnd - beltStart;
        int yEvery = amt.y == 0 ? INT_MAX : (abs(amt.x) + abs(amt.z)) / abs(amt.y);
        if(yEvery == 0) amt = ivec3(0);
        int totalRails = abs(amt.x) + abs(amt.z);
        int delayedY = 0;
        int railIndex = 0;
        ivec3 firstRail = ivec3(-1);
        ivec3 lastRail = ivec3(-1);
#define yCondition yEvery != INT_MAX && abs(amt.y) > 0

        while(abs(amt.x) > 0) {
            int railType = NORTH * SOUTH;
            bool isTurn = beltStart + amt != beltEnd && beltEnd.x - beltStart.x == amt.x && abs(beltEnd.z - beltStart.z) > 0;
            if(isTurn) railType = (dir.x > 0 ? SOUTH : NORTH) * (dir.z > 0 ? EAST : WEST);
            bool blockY = isTurn;
            if(delayedY > 0 && yCondition) {
                if(blockY) delayedY++;
                else {
                    amt -= ivec3(0, dir.y, 0);
                    delayedY--;
                }
            }
            else if(yCondition && railIndex % yEvery == yEvery / 2) {
                if(blockY) delayedY++;
                else {
                    amt -= ivec3(0, dir.y, 0);
                    railType += dir.x * -dir.y == 1 ? UP_FIRST : UP_SECOND;
                }
            }
            if(railType > 1 && railType % 2 == 1) {
                if(beltEnd.y > beltStart.y) {
                    setBlock(pos + amt + ivec3(0, 1, 0), Block(AIR));
                    setBlock(pos + amt,  Block(RAIL, railType));
                } else {
                    setBlock(pos + amt, Block(AIR));
                    setBlock(pos + amt - ivec3(0, 1, 0), Block(RAIL, railType));
                }

            } else setBlock(pos + amt, Block(RAIL, railType));
            if(railIndex == totalRails - 1) firstRail = pos + amt;
            if(railIndex == 1) lastRail = pos + amt;
            amt -= ivec3(dir.x, 0, 0);
            railIndex++;
        }

        while(abs(amt.z) > 0) {
            int railType = EAST * WEST;
            bool isTurn = beltStart + amt != beltEnd && beltEnd.z - beltStart.z == amt.z && abs(beltEnd.x - beltStart.x) > 0;
            if(isTurn) railType = (dir.x > 0 ? SOUTH : NORTH) * (dir.z > 0 ? EAST : WEST);
            bool blockY = isTurn;
            if(delayedY > 0 && yCondition) {
                if(blockY) delayedY++;
                else {
                    amt -= ivec3(0, dir.y, 0);
                    delayedY--;
                }
            }
            else if(yCondition && railIndex % yEvery == yEvery / 2) {
                if(blockY) delayedY++;
                else {
                    amt -= ivec3(0, dir.y, 0);
                    railType += dir.z * -dir.y == 1 ? UP_FIRST : UP_SECOND;
                }
            }

            if(railType > 1 && railType % 2 == 1) {
                if(beltEnd.y > beltStart.y) {
                    setBlock(pos + amt + ivec3(0, 1, 0), Block(AIR));
                    setBlock(pos + amt,  Block(RAIL, railType));
                } else {
                    setBlock(pos + amt, Block(AIR));
                    setBlock(pos + amt - ivec3(0, 1, 0), Block(RAIL, railType));
                }
            } else setBlock(pos + amt, Block(RAIL, railType));
            if(railIndex == totalRails - 1) firstRail = pos + amt;
            if(railIndex == 1) lastRail = pos + amt;
            amt -= ivec3(0, 0, dir.z);
            railIndex++;
        }

        if(firstRail != ivec3(-1)) {
            if(beltStart.x > firstRail.x) setBlock(beltStart, Block(RAIL, NORTH * NORTH));
            else if(beltStart.x < firstRail.x) setBlock(beltStart, Block(RAIL, SOUTH * SOUTH));
            else if(beltStart.z > firstRail.z) setBlock(beltStart, Block(RAIL, EAST * EAST));
            else if(beltStart.z < firstRail.z) setBlock(beltStart, Block(RAIL, WEST * WEST));
            else std::cout << "PANIC! THE RAILS ARE TELEPORTING" << std::endl;
        }
        if(lastRail != ivec3(-1) && !(getBlock(lastRail)->data > 1 && getBlock(lastRail)->data % 2 == 1)) {
            if(beltEnd.x > lastRail.x) setBlock(beltEnd, Block(RAIL, NORTH * NORTH));
            else if(beltEnd.x < lastRail.x) setBlock(beltEnd, Block(RAIL, SOUTH * SOUTH));
            else if(beltEnd.z > lastRail.z) setBlock(beltEnd, Block(RAIL, EAST * EAST));
            else if(beltEnd.z < lastRail.z) setBlock(beltEnd, Block(RAIL, WEST * WEST));
            else std::cout << "PANIC! THE RAILS ARE TELEPORTING" << std::endl;
        }
        beltStart = ivec3(-1);
    }
    if(window->isInputClicked(GLFW_KEY_C)) cart.setBlock(hoverBlock);


    //bool newHoverBlock = lastHoverBlock != hoverBlock;
    //lastHoverBlock = hoverBlock;
    /*if(hoverBlock != vec3(-1)) {
        //if((isRightMouseDown && newHoverBlock) || isRightMouseClick) {
            //Block* lastBlock = nullptr;

            /*block->type = BELT;
            if(lastBelt == vec3(-1)) {
                block->setBeltFrom(DEFAULT);
                block->setBeltTo(DEFAULT);
            } else {
                lastBlock = &world[static_cast<int>(lastBelt.y)][static_cast<int>(lastBelt.x)][static_cast<int>(lastBelt.z)];
            }
            if(lastBlock != nullptr) {
                if(hoverBlock.x > lastBelt.x) {
                    block->setBeltFrom(NORTH);
                    lastBlock->setBeltTo(SOUTH);
                } else if(hoverBlock.x < lastBelt.x) {
                    block->setBeltFrom(SOUTH);
                    lastBlock->setBeltTo(NORTH);
                } else if(hoverBlock.z > lastBelt.z) {
                    block->setBeltFrom(EAST);
                    lastBlock->setBeltTo(WEST);
                } else if(hoverBlock.z < lastBelt.z) {
                    block->setBeltFrom(WEST);
                    lastBlock->setBeltTo(EAST);
                }
            }*/

            /*if(block->type != RAIL) block->type = RAIL;
            bool found = false;
            for(int i = 0; i < 13; i++) {
                if(block->data == blockVariants[RAIL][i]) {
                    block->data = blockVariants[RAIL][i + 1];
                    found = true;
                    break;
                }
            }
            if(!found) block->data = blockVariants[RAIL][0];
            cart.setBlock(hoverBlock);

            lastBelt = hoverBlock;*/
        //}
    //}

    fontRenderer.setColor(vec4(1, 0.5f, 1, 1));
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << cart.speed;
    fontRenderer.draw(
        "\n\nCart:\nPos:" +
        std::to_string(static_cast<int>(cart.blockPos.x)) + ", " + std::to_string(static_cast<int>(cart.blockPos.y)) + ", " + std::to_string(static_cast<int>(cart.blockPos.z)) + "\nProgress:" +
        std::to_string(cart.progress) +"\nSpeed:" +
        out.str() + "\n" +
        (cart.backwards ? "backwards" : "forwards"),
        10, Window::GAME_HEIGHT - 10 - fontRenderer.getHeight() * 3 * fontScale, fontScale);


}

void WorldScene::cleanup() {
    if(world != nullptr) {
        for(int i = 0; i < WORLD_SIZE; i++) {
            for(int j = 0; j < WORLD_SIZE; j++) {
                delete [] world[i][j];
            }
            delete [] world[i];
        }
        delete [] world;
        world = nullptr;
    }

    for(const auto&[fst, snd] : blockVariants) delete [] snd;
    blockVariants.clear();
}

void WorldScene::keyPress(int key, int action, int mods) {
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
        } else if(key == GLFW_KEY_P) {
            for(int y = 0; y < WORLD_SIZE; y++) {
                for(int x = 0; x < WORLD_SIZE; x++) {
                    for(int z = 0; z < WORLD_SIZE; z++) {
                        std::cout << world[y][x][z].type << ";" << world[y][x][z].data << " ";
                    }
                    std::cout << "| ";
                }
                std::cout << "* ";
            }
            std::cout << std::endl;
        }
    }
}

void WorldScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}

void WorldScene::scrollWheel(float xOff, float yOff) {
    zoom += yOff;
    if(zoom < 0) zoom = 0;
    if(zoom > WORLD_SIZE * 2 - 1) zoom = WORLD_SIZE * 2 - 1;
}
