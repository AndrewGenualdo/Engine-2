//
// Created by cobble on 7/11/2025.
//

#include "pathing.h"



Camera PathingScene::camera = Camera();
Window *PathingScene::window = nullptr;
FontRenderer PathingScene::fontRenderer = FontRenderer();
Texture2d PathingScene::blank = Texture2d();
std::vector<PathingScene::Wall> PathingScene::walls = std::vector<Wall>();

PathingScene::PathingScene(Window *w) {
    window = w;
}

void PathingScene::load() {
    window->setWindowTitle("Pathfinding????");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    fontRenderer = FontRenderer("assets/textures/font/font.png");
    blank = Texture2d("assets/textures/ui/blank.png");
    walls = std::vector<Wall>();
    for(int i = 0; i < MAX_UNITS; i++) {
        units[i] = Unit(vec2(i * 20), 40 - i, 500 - static_cast<float>(i) / MAX_UNITS);
        units[i].unit._color = vec4(vec3(1 - static_cast<float>(i) / MAX_UNITS), 1);
    }
}

bool wasLeftDown = false;
bool wasRightDown = false;
vec2 wallStart = vec2(0);

void PathingScene::draw() {
    float deltaTime = window->update();
    camera.update(window->window, deltaTime);
    if(glfwGetKey(window->window, GLFW_KEY_SPACE)) deltaTime = 0;
    else if(glfwGetKey(window->window, GLFW_KEY_F)) {
        deltaTime *= 3;
        if(glfwGetKey(window->window, GLFW_KEY_LEFT_CONTROL)) {
            deltaTime *= 3;
        }
    }
    drawBackground(true);
    Texture2d::gameCamera.reset();
    Texture2d::gameCamera.expandToInclude(0, 0);
    Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);
    float mx = window->mousePos.x;
    float my = window->mousePos.y;

    for(int i = 0; i < walls.size(); i++) {
        walls[i].wall._color = vec4(242, 78, 110, 255) / 255.0f;
        walls[i].wall.draw(true);
        float radius = units[0].unit.size();
        Line2d wall = walls[i].wall;
        vec2 dir = normalize(wall._end - wall._start);
        vec2 right = units[0].perpendicularRight(dir);
        vec2 left = -right;

        vec2 startBot = wall._start + right * radius; //red
        vec2 startTop  = wall._start + left * radius; //green
        vec2 endBot = wall._end + right * radius; //blue
        vec2 endTop  = wall._end + left * radius; //magenta
        Circle2d sbCircle = Circle2d(startBot, 10, 0.2, vec4(1, 0, 0, 1));
        sbCircle.draw(true);
        Circle2d stCircle = Circle2d(startTop, 10, 0.2, vec4(0, 1, 0, 1));
        stCircle.draw(true);
        Circle2d ebCircle = Circle2d(endBot, 10, 0.2, vec4(0, 0, 1, 1));
        ebCircle.draw(true);
        Circle2d etCircle = Circle2d(endTop, 10, 0.2, vec4(1, 0, 1, 1));
        etCircle.draw(true);

    }

    if(glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_RIGHT) && !wasRightDown) {
        vec2 goToPoint = vec2(mx, my);
        for(int i = 0; i < walls.size(); i++) {
            vec2 p = ew::closestPointOnLine(walls[i].wall._start, walls[i].wall._end, vec2(mx, my));
            if(distance(p, vec2(mx, my)) < units[0].unit.size()) {
                vec2 dir = normalize(vec2(mx, my) - p);
                goToPoint = p + dir * units[0].unit.size();
            }
        }
        //units[0].pathfinding.clear();
        units[0].getToPoint(goToPoint, deltaTime == 0 ? 1.0f / 60.0f : deltaTime);
        //units[0].buildPath(units[0].unit.pos(), goToPoint);


    }

    for(int i = 1; i < MAX_UNITS; i++) {
        units[i].pathfinding.clear();
        units[i].pathfinding.push_back(units[i - 1].unit.pos());
    }

    if(glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) && !wasLeftDown) wallStart = vec2(mx, my);
    if(!glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) && wasLeftDown && !(wallStart.x == 0 && wallStart.y == 0) && length(wallStart - vec2(mx, my)) > 10) walls.push_back(Wall(wallStart, vec2(mx, my)));
    wasRightDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_RIGHT);
    wasLeftDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT);
    if(wasLeftDown) {
        Line2d wall = Line2d(wallStart, vec2(mx, my), vec4(242, 78, 110, 255) / 255.0f);
        wall.draw(true);
    }

    for(int i = 0; i < MAX_UNITS; i++) {
        units[i].unit.draw(true);
        if(units[i].pathfinding.size() > 0) {
            vec2 move = units[i].pathfinding[units[i].pathfinding.size() - 1] - units[i].unit.pos();
            if(length(move) < units[i].speed * deltaTime) {
                units[i].unit.setPos(units[i].pathfinding[units[i].pathfinding.size() - 1]);
                units[i].pathfinding.pop_back();
            }
            else units[i].unit.setPos(units[i].unit.pos() + normalize(move) * units[i].speed * deltaTime);

            for(int j = units[i].pathfinding.size(); j > 0; j--) {
                Line2d path;
                if(j == units[i].pathfinding.size()) path = Line2d(units[i].unit.pos(), units[i].pathfinding[j - 1], vec4(1));
                else path = Line2d(units[i].pathfinding[j], units[i].pathfinding[j - 1], vec4(1));
                float darkness = 1 - static_cast<float>(i) / MAX_UNITS;
                int colorIndex = j % 6;
                vec4 c;
                switch(colorIndex) {
                    case 0: c = vec4(1,0,0,1); break;
                    case 1: c = vec4(1,1,0,1); break;
                    case 2: c = vec4(0,1,0,1); break;
                    case 3: c = vec4(0,1,1,1); break;
                    case 4: c = vec4(0,0,1,1); break;
                    case 5: c = vec4(1,0,1,1); break;
                    default: c = vec4(1); break;
                }
                path._color = vec4(vec3(c.r, c.g, c.b) * darkness, 1.0f);


                path.draw(true);
            }
        }
    }

}

void PathingScene::cleanup() {

}

void PathingScene::keyPress(int key, int action, int mods) {

}

void PathingScene::mouseMove(float x, float y) {
    window->mousePos.x = (x - window->gx) * Window::GAME_WIDTH / window->gw;
    window->mousePos.y = Window::GAME_HEIGHT - ((y - window->gy) * Window::GAME_HEIGHT / window->gh);
}

void PathingScene::drawBackground(bool pride) {
    float visibility = 0.05f * 3;
    vec4 blue = vec4(91, 206, 250, visibility * 255.0f) / 255.0f;
    vec4 pink = vec4(245, 169, 184, visibility * 255.0f) / 255.0f;
    vec4 white = vec4(255, 255, 255, visibility * 255.0f) / 255.0f;

    float x = window->gx;
    float y = window->gy;
    float w = window->gw;
    float h = window->gh;

    Texture2d::setColor(vec4(0, 0, 0, 1));
    blank.drawRaw(x, y, w, h, true);

    if(pride) {
        Texture2d::setColor(blue);
        blank.drawRaw(x, y + (h * 0.0f), w, h * 0.2f);
        Texture2d::setColor(pink);
        blank.drawRaw(x, y + (h * 0.2f), w, h * 0.2f);
        Texture2d::setColor(white);
        blank.drawRaw(x, y + (h * 0.4f), w, h * 0.2f);
        Texture2d::setColor(pink);
        blank.drawRaw(x, y + (h * 0.6f), w, h * 0.2f);
        Texture2d::setColor(blue);
        blank.drawRaw(x, y + (h * 0.8f), w, h * 0.2f);
    }

}
