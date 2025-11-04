//
// Created by cobble on 1/11/2025.
//

#include "comic.h"
#include "cobb/misc/texture2d.h"
#include "cobb/misc/fontRenderer.h"

Camera ComicScene::camera = Camera();
Window *ComicScene::window = nullptr;

ComicScene::ComicScene(Window *w) : Scene(window) {
    ComicScene::window = w;
}

Texture2d comicPage;
Texture2d blank;
FontRenderer fontRenderer2;

void ComicScene::load() {
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    blank = Texture2d("assets/textures/ui/blank.png");
    comicPage = Texture2d("assets/textures/Act Like You Love Me!/55-0.jpg");
    fontRenderer2 = FontRenderer("assets/textures/font/font.png");
}

float offset = 0.0f;

void ComicScene::draw() {
    float deltaTime = window->update();
    camera.update(window->window, deltaTime);

    if (glfwGetKey(window->window, GLFW_KEY_UP)) {
        offset += 1;
    } else if (glfwGetKey(window->window, GLFW_KEY_DOWN)) {
        offset -= 1;
    }

    Texture2d::gameCamera.reset();
    Texture2d::gameCamera.expandToInclude(0, 0);
    Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);
    Texture2d::gameCamera.adjustToAspectRatio(static_cast<float>(Window::GAME_WIDTH) / static_cast<float>(Window::GAME_HEIGHT));

    Texture2d::setColor(vec4(0.0f, 0.1f, 0.2f, 1.0f));
    blank.drawRaw(window->gx, window->gy, window->gw, window->gh);
    Texture2d::setColor(vec4(1));

    float goalWidth = 500.0f;
    float widthMult = comicPage.getWidth() / goalWidth;
    float newHeight = comicPage.getHeight() / widthMult;
    comicPage.drawPartial(125, Window::GAME_HEIGHT - 300, 0, offset + 1595, goalWidth, 2100, goalWidth, newHeight);
    fontRenderer2.setColor(vec4(0.5f, 1, 0.7f, 1));
    fontRenderer2.draw("Series Title:\nEpisode: 55-2", 0, fontRenderer2.getHeight() * 5 + 15, 5);
}

void ComicScene::keyPress(int key, int action, int mods) {
    Scene::keyPress(key, action, mods);
}

void ComicScene::mouseMove(float x, float y) {
    Scene::mouseMove(x, y);
}
