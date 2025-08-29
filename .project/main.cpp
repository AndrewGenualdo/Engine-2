#include <cstdio>
#include "cobb/core/window.h"
#include "ew/external/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "cobb/misc/texture2d.h"
#include "cobb/misc/animation.h"

#include "cobb/misc/fontRenderer.h"
#include "scenes/world/world.h"

#include <thread>

#include "cobb/shapes/circle2d.h"
#include "cobb/shapes/line2d.h"
#include "scenes/game/client.h"

using namespace cobb;


static Window window;

static Scene *activeScene;

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    activeScene->keyPress(key, action, mods);
}

void mouse_position_callback(GLFWwindow *w, double xpos, double ypos) {
    activeScene->mouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
}

void scroll_callback(GLFWwindow *w, double xoffset, double yoffset) {
    //camera.handleScroll(yoffset);
    activeScene->scrollWheel(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void framebuffer_size_callback(GLFWwindow *w, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    window.setWidth(width);
    window.setHeight(height);
    glViewport(0, 0, width, height);
    Texture2d::setOrtho(glm::ortho(0.0f, window.getWidth(), 0.0f, window.getHeight(), -1.0f, 1.0f));
    if (Client::debug) std::cout << "Window resized to: " << window.getWidth() << "x" << window.getHeight() << "." << std::endl;
    window.getGameDimensions();
    if (Client::debug) std::cout << "Game window resized to: " << window.gw << "x" << window.gh << "." << std::endl;
}

int main() {
    window = Window("Loading...");

    Window::setVsync(true);
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
    Texture2d::loadShader();
    Texture2d::setOrtho(glm::ortho(0.0f, Window::SCREEN_WIDTH, 0.0f, Window::SCREEN_HEIGHT, -1.0f, 1.0f));
    Texture2d::window = &window;
    MultiTexture2d::loadShader();
    FontRenderer::loadShader();
    Line2d::loadShader();
    Line2d::line2dShader->setMat4("projection", Texture2d::orthoProj);
    Circle2d::loadShader();
    Circle2d::circle2dShader->setMat4("projection", Texture2d::orthoProj);


    activeScene = new WorldScene(&window);
    activeScene->load();

    //std::thread serv = std::thread(Server::run);
    //std::thread clie = std::thread(Client::run);

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();

        activeScene->draw();

        glfwSwapBuffers(window.window);
    }
    printf("Shutting down...");
    activeScene->cleanup();
    delete activeScene;
}
