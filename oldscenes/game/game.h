//
// Created by cobble on 12/28/2024.
//

#ifndef ENGINE_2_GAME_H
#define ENGINE_2_GAME_H

#include "cobb/core/scene.h"
#include "chatBox.h"
#include "cobb/core/camera.h"

using namespace glm;
using namespace cobb;

class GameScene : public Scene {

public:
    static Camera camera;
    static Window *window;

    GameScene() = default;
    explicit GameScene(Window *w);

    void load() override;
    void draw() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;

private:
    ChatBox chatBox;
    bool editMode = false;
    bool borderless = false;
    FontRenderer fontRenderer;
    Texture2d platform;
    bool wasMouseDown = false;
    bool wasRightMouseDown = false;
    vec2 mouseDownStart = vec2(-1);
};

#endif //ENGINE_2_GAME_H
