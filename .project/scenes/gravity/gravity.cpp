//
// Created by cobble on 8/26/2025.
//

#include "gravity.h"
//
// Created by cobble on 8/19/2025.
//

#include "gravity.h"

#include "cobb/shapes/line2d.h"
#include "ew/ewMath/ewMath.h"


Camera GravityScene::camera = Camera();
Window *GravityScene::window = nullptr;
FontRenderer GravityScene::fontRenderer = FontRenderer();
Texture2d GravityScene::blank = Texture2d();
bool GravityScene::debugMode = false;

GravityScene::GravityScene(Window *w) {
    window = w;
}

GravityScene::~GravityScene() {
    cleanup();
}

constexpr static int COLOR_COUNT = 10000;
vec3 *colors = new vec3[COLOR_COUNT];

void GravityScene::load() {
    window->setWindowTitle("Gravity Scene");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");

    for(int i = 0; i < COLOR_COUNT; i++) {
        colors[i] = vec3(ew::RandomRange(0, 1), ew::RandomRange(0, 1), ew::RandomRange(0, 1));
    }
}





void GravityScene::draw() {
    float deltaTime = window->update();
    camera.update(window->window, deltaTime);
    float fontScale = 3.0f;

    //do background stuff here
    Texture2d::setColor(vec4(0, 0, 0, 1));
    blank.drawRaw(window->gx, window->gy, window->gw, window->gh, true);

    Texture2d::gameCamera.reset();
    Texture2d::gameCamera.expandToInclude(0, 0);
    Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);

    bool isLeftMouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT);
    bool isLeftMouseClick = isLeftMouseDown && !wasLeftMouseDown;
    bool isLeftMouseRelease = !isLeftMouseDown && wasLeftMouseDown;
    wasLeftMouseDown = isLeftMouseDown;

    bool isRightMouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_RIGHT);
    bool isRightMouseClick = isRightMouseDown && !wasRightMouseDown;
    bool isRightMouseRelease = !isRightMouseDown && wasRightMouseDown;
    wasRightMouseDown = isRightMouseDown;

    float mx = window->mousePos.x;
    float my = window->mousePos.y;

    Texture2d::setColor(vec4(1.0f, 0.5f, 0.5f, 1.0f));
    //blank.draw(mx - 50, my - 50, 100.0f, 100.0f);


    if(isLeftMouseClick || isRightMouseClick) {
        mouseStart = vec2(mx, my);
    }
    if(isLeftMouseDown) {
        Circle2d tempCircle = Circle2d(mouseStart, distance(vec2(mx, my), mouseStart), 0, vec4(0.5f));
        tempCircle.draw(true);
    }
    if(isLeftMouseRelease) {
        objects.push_back(Obj(mouseStart, vec2(0), distance(vec2(mx, my), mouseStart)));
    }
    if(isRightMouseDown) {
        vec4 col = vec4(colors[objects.size() % COLOR_COUNT], 1);
        Circle2d tempCircle = Circle2d(mouseStart, 10.0f, 0, col);
        tempCircle.draw(true);
        Line2d tempLine = Line2d(mouseStart, vec2(mx, my), col);
        tempLine.draw(true);
    }
    if(isRightMouseRelease) {
        objects.push_back(Obj(mouseStart, (mouseStart - vec2(mx, my)) / 100.0f, 10.0f));
        objects[objects.size() - 1].locked = false;
    }
    if(glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_MIDDLE)) {
        objects.push_back(Obj(vec2(mx, my), vec2(0), 10.0f));
    }

    for(int i = objects.size() - 1; i >= 0; i--) {
        for(int j = 0; j < objects.size(); j++) {
            if(i != j) {
                objects[i].vel += GRAVITY * (objects[j].pos - objects[i].pos) * objects[j].mass / objects[i].mass;
                float dist = distance(objects[i].pos, objects[j].pos);
                float goalDist = objects[i].mass + objects[j].mass;
                if(dist < goalDist) {
                    vec2 tempVel = objects[j].vel * objects[j].mass / objects[i].mass * FRICTION;
                    objects[j].vel = objects[i].vel * objects[i].mass / objects[j].mass * FRICTION;
                    objects[i].vel = tempVel;
                    if(dist == 0) {
                        objects[i].pos.x += goalDist;
                    } else {
                        vec2 dir = normalize(objects[i].pos - objects[j].pos) * (goalDist - dist);
                        objects[i].pos += dir;
                        //dir = normalize(dir) * goalDist - objects[i].pos;
                        //objects[i].pos += dir;
                    }


                }
            }
        }

    }

    for(int i = 0; i < objects.size(); i++) {
        objects[i].update();
        //if(objects[i].pos.x > Window::GAME_WIDTH || objects[i].pos.y > Window::GAME_HEIGHT || objects[i].pos.x < 0 || objects[i].pos.y < 0) objects.erase(objects.begin() + i);
        Circle2d obj = Circle2d(objects[i].pos, objects[i].mass, 0, vec4(colors[i%COLOR_COUNT], 1));
        obj.draw(true);
    }

    fontRenderer.setColor(vec4(1));
    fontRenderer.draw(std::to_string(objects.size()), 10, Window::GAME_HEIGHT - 10 - fontRenderer.getHeight() * fontScale, fontScale);
}

void GravityScene::cleanup() {

}

void GravityScene::keyPress(int key, int action, int mods) {
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

void GravityScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}
