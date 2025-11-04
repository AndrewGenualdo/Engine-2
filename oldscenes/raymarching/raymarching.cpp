//
// Created by cobble on 9/16/2025.
//

#include "raymarching.h"

#include "cobb/shapes/circle2d.h"
#include "cobb/shapes/line2d.h"
#include "ew/ewMath/ewMath.h"

Camera RaymarchingScene::camera = Camera();
Window *RaymarchingScene::window = nullptr;
FontRenderer RaymarchingScene::fontRenderer = FontRenderer();
Texture2d RaymarchingScene::blank = Texture2d();
bool RaymarchingScene::debugMode = false;

RaymarchingScene::RaymarchingScene(Window *w) {
    window = w;
}

RaymarchingScene::~RaymarchingScene() {
    cleanup();
}

struct Obj {

    Obj() = default;
    virtual ~Obj() = default;

    virtual vec2 getClosestPoint(vec2 point) {
        return vec2(0);
    }
};

struct Circle : Obj {
    Circle2d circle;

    Circle() {
        circle = Circle2d();
    }

    Circle(const Circle2d &circle) {
        this->circle = circle;
    }
    ~Circle() = default;

    vec2 getClosestPoint(vec2 point) override {
        vec2 p = normalize(point - circle.pos()) * circle.size() + circle.pos();
        return p;
    }
};
constexpr int objCount = 20;
Circle *objects = nullptr;


void RaymarchingScene::load() {
    window->setWindowTitle("Raymarching Scene");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");

    objects = new Circle[20];
    for(int i = 0; i < objCount; i++) {
        objects[i] = Circle(Circle2d(vec2(ew::RandomRange(0, Window::GAME_WIDTH), ew::RandomRange(0, Window::GAME_HEIGHT)), ew::RandomRange(5, 25), 0, vec4(ew::RandomRange(0, 1), ew::RandomRange(0, 1), ew::RandomRange(0, 1), 1)));
    }

}


void RaymarchingScene::draw() {
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
    Circle2d start = Circle2d(vec2(Window::GAME_WIDTH / 2, Window::GAME_HEIGHT / 2), 10.0f, 0, vec4(1));
    start.draw();


    for(int i = 0; i < objCount; i++) {
        objects[i].circle.draw();

    }
    Line2d line = Line2d(start.pos(), vec2(mx, my), vec4(1));
    line.draw();

    vec2 raypoint = start.pos();
    for(int i = 0; i < 100; i++) {
        float closestDist = 99999999999;
        for(int j = 0; j < objCount; j++) {
            float dist = distance(objects[j].getClosestPoint(raypoint), raypoint);
            if(dist < closestDist) {
                closestDist = dist;
            }
        }
        Circle2d rayCircle = Circle2d(raypoint, closestDist, 0.025f, vec4(1));
        rayCircle.setGlow(0);
        rayCircle.draw();
        raypoint += normalize(vec2(mx, my) - raypoint) * closestDist;
    }


    //Texture2d::setColor(vec4(1.0f, 0.5f, 0.5f, 1.0f));
    //blank.draw(mx - 50, my - 50, 100.0f, 100.0f);
}

void RaymarchingScene::cleanup() {
    if(objects != nullptr) {
        delete [] objects;
        objects = nullptr;
    }
}

void RaymarchingScene::keyPress(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if(key == GLFW_KEY_R) {
            cleanup();
            load();
        } else if(key == GLFW_KEY_F8) {
            debugMode = !debugMode;
        }
    }
}

void RaymarchingScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}
