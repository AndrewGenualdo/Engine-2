//
// Created by cobble on 8/19/2025.
//

#ifndef TEMPLATE_H
#define TEMPLATE_H
#include "cobb/core/camera.h"
#include "cobb/core/scene.h"
#include "cobb/misc/fontRenderer.h"
#include "cobb/misc/texture2d.h"


class TemplateScene : public Scene {
public:
    static Camera camera;
    static Window *window;

    TemplateScene() = default;
    explicit TemplateScene(Window *w);
    virtual ~TemplateScene();

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;

    static FontRenderer fontRenderer;
    static Texture2d blank;
    static bool debugMode;

};



#endif //TEMPLATE_H
