//
// Created by cobble on 11/3/2025.
//

#ifndef ENGINE_2_FARMINGGAME_H
#define ENGINE_2_FARMINGGAME_H


#include "cobb/core/camera.h"
#include "cobb/core/scene.h"
#include "cobb/misc/fontRenderer.h"
#include "cobb/misc/multiTexture2d.h"
#include "cobb/misc/tiles2d.h"
#include "farmingWorld.h"

class FarmingScene : public Scene {
public:
    static Camera camera;
    static Window *window;

    FarmingScene() = default;
    explicit FarmingScene(Window *w);
    ~FarmingScene() override;

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;

    static FontRenderer fontRenderer;
    static Texture2d blank;
    //static MultiTexture2d landTextures;
    //static MultiTexture2d structureTextures;

    //static Texture2d guy;
    static FarmingWorld world;

    static bool debugMode;

};

#endif //ENGINE_2_FARMINGGAME_H