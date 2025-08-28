//
// Created by cobble on 8/26/2025.
//

#ifndef GRAVITY_H
#define GRAVITY_H

#include "cobb/core/camera.h"
#include "cobb/core/scene.h"
#include "cobb/misc/fontRenderer.h"
#include "cobb/misc/texture2d.h"
#include <vector>
#include "cobb/shapes/circle2d.h"

class GravityScene : public Scene {
public:
    static Camera camera;
    static Window *window;

    GravityScene() = default;
    explicit GravityScene(Window *w);
    virtual ~GravityScene();

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;

    static FontRenderer fontRenderer;
    static Texture2d blank;
    static bool debugMode;

    struct Obj {
        vec2 pos;
        vec2 vel;
        float mass;
        bool locked = true;

        Obj(vec2 pos, vec2 vel, float mass) {
            this->pos = pos;
            this->vel = vel;
            this->mass = mass;
        }

        void update() {
            //if(locked) return;
            pos += vel;
        }
    };

    constexpr static float GRAVITY = 0.000001f;
    constexpr static float FRICTION = 0.99f;

    bool wasLeftMouseDown = false;
    bool wasRightMouseDown = false;
    std::vector<Obj> objects = std::vector<Obj>();
    vec2 mouseStart = vec2(-1);
};



#endif //GRAVITY_H
