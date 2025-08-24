//
// Created by cobble on 1/11/2025.
//

#ifndef ENGINE_2_COMIC_H
#define ENGINE_2_COMIC_H


#include "cobb/core/scene.h"
#include "cobb/core/camera.h"

using namespace cobb;

class ComicScene : public Scene {
public:
    static Camera camera;
    static Window *window;

    ComicScene() = default;
    explicit ComicScene(Window *w);

    void load() override;
    void draw() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;
};


#endif //ENGINE_2_COMIC_H
