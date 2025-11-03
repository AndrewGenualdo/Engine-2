//
// Created by cobble on 9/16/2025.
//

#ifndef RAYMARCHING_H
#define RAYMARCHING_H

#include "cobb/core/camera.h"
#include "cobb/core/scene.h"
#include "cobb/misc/fontRenderer.h"


class RaymarchingScene : public Scene {
public:
    static Camera camera;
    static Window *window;

    RaymarchingScene() = default;
    explicit RaymarchingScene(Window *w);
    ~RaymarchingScene() override;

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;

    static FontRenderer fontRenderer;
    static Texture2d blank;
    static bool debugMode;
};



#endif //RAYMARCHING_H
