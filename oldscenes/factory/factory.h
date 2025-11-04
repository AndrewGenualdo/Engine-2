//
// Created by cobble on 8/19/2025.
//

#pragma once

#ifndef FACTORY_H
#define FACTORY_H



#include "cobb/core/camera.h"
#include "cobb/core/scene.h"
#include "cobb/misc/fontRenderer.h"
#include "cobb/misc/texture2d.h"
#include "items/item.h"
#include <vector>

#include "cobb/misc/animation.h"
#include "structures/structure.h"
#include "structures/miner.h"
#include "structures/collector.h"
#include "items/rock.h"

class Structure;


class FactoryScene : public Scene {
public:
    static Camera camera;
    static Window *window;

    FactoryScene() = default;
    explicit FactoryScene(Window *w);
    virtual ~FactoryScene() override;

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;

    static FontRenderer fontRenderer;
    static Texture2d blank;
    static bool debugMode;

    static std::vector<Item*> items;
    static std::vector<Item*> itemsToDestroy;
    static Structure ***structs;

    static int score;

    static Texture2d wallpaper;

    constexpr static vec2 GRID_DIMS = vec2(17, 9);
    constexpr static vec2 TILE_DIMS = vec2(100, 100);
    constexpr static vec2 GRID_OFFSET = vec2(Window::GAME_WIDTH * 0.5f - (TILE_DIMS.x * GRID_DIMS.x * 0.5f), Window::GAME_HEIGHT * 0.5f - (TILE_DIMS.y * GRID_DIMS.y * 0.5f));
    constexpr static float GRAVITY = 500.0f;

    static vec2 getTileAtPos(vec2 pos);
    static vec2 tilePos(vec2 tile);

    static Structure* getTile(vec2 pos);
    static void destroyTile(vec2 pos);
    static void setTile(vec2 pos, Structure *structure);
    static bool isTile(vec2 tile);
    static bool isTileFull(vec2 tile);


};



#endif //TEMPLATE_H
