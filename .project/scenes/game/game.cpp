//
// Created by cobble on 1/11/2025.
//

#include "game.h"

Camera GameScene::camera = Camera();
Window *GameScene::window = nullptr;

GameScene::GameScene(Window *w) {
    GameScene::window = w;
}



void GameScene::load() {
    window->setWindowTitle("Platformer!!!");
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window->getWidth(), window->getHeight()));
    Client::paused = &camera.lock;
    for (int i = 0; i < Player::MAX_PLAYERS; i++) {
        Player::players[i] = Player(window, "assets/textures/guy/", 0.5f);
        Player::players[i].id = i;
    }





    for (int i = 0; i <= Player::MAX_PLAYERS; i++) fontRenderer.colors[i] = Player::playerColors[i];
    chatBox = ChatBox(&fontRenderer, 2.0f);

    fontRenderer = FontRenderer("assets/textures/font/font.png");
    platform = Texture2d("assets/textures/ui/blank.png");
    Player::levels[Player::currentLevel].load("assets/levels/1.txt");
}

/*void drawAxisGizmo() {
	if(camera.ui && lineShader != nullptr) {
		glDisable(GL_DEPTH_TEST);

		//higher number = less stuttering, can't be above ~950 because depth will start cutting it off, 900 to be safe
		const float compassScale = 900.0f;

		vec3 p = camera.forward * vec3(compassScale) + camera.pos;

		Line xAxis = Line(p, p + vec3(0.05f * compassScale, 0, 0), vec4(1, 0, 0, 1));
		Line yAxis = Line(p, p + vec3(0, 0.05f * compassScale, 0), vec4(0, 1, 0, 1)); //up
		Line zAxis = Line(p, p + vec3(0, 0, 0.05f * compassScale), vec4(0, 0, 1, 1));

		lineShader->use();
		lineShader->setMat4("viewProj", camera.proj * camera.view);
		glBindVertexArray(*Line::getVAO());

		xAxis.draw(&camera);
		yAxis.draw(&camera);
		zAxis.draw(&camera);

		glEnable(GL_DEPTH_TEST);
	}
}*/

void GameScene::draw() {
    float deltaTime = window->update();
    camera.update(window->window, deltaTime);
    mat4 viewProj = camera.proj * camera.view;
    bool mouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT);
    bool rightMouseDown = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_RIGHT);
    bool mousePressed = mouseDown && !wasMouseDown;
    bool rightMousePressed = rightMouseDown && !wasRightMouseDown;
    bool mouseReleased = wasMouseDown && !mouseDown;
    bool rightMouseReleased = wasRightMouseDown && !rightMouseDown;
    wasMouseDown = mouseDown;
    wasRightMouseDown = rightMouseDown;
    if (mousePressed || rightMousePressed) mouseDownStart = vec2(window->mousePos.x, window->mousePos.y);


    //game camera management MUST BE DONE BEFORE ANY DRAW CALLS
    float lastGCX, lastGCY, lastGCW, lastGCH;
    Texture2d::gameCamera.copy(&lastGCX, &lastGCY, &lastGCW, &lastGCH);
    Texture2d::gameCamera.reset();
    for (int i = 0; i < Player::MAX_PLAYERS; i++) {
        if (!Player::players[i].connected) continue;
        //player physics
        if (!(camera.lock && i == Client::id)) Player::players[i].update(deltaTime, Player::levels[Player::currentLevel]);
        Texture2d::gameCamera.expandToInclude(Player::players[i].pos.x - 150, Player::players[i].pos.y - 150);
        Texture2d::gameCamera.expandToInclude(Player::players[i].pos.x + Player::players[i].hurtbox.w + 150, Player::players[i].pos.y + Player::players[i].hurtbox.h + 150);
    }
    float gcLerp = min(5.0f * deltaTime, 1.0f);
    Texture2d::gameCamera.set(mix(lastGCX, Texture2d::gameCamera.x, gcLerp), mix(lastGCY, Texture2d::gameCamera.y, gcLerp), mix(lastGCW, Texture2d::gameCamera.w, gcLerp), mix(lastGCH, Texture2d::gameCamera.h, gcLerp));
    Texture2d::gameCamera.adjustToAspectRatio(static_cast<float>(Window::GAME_WIDTH) / static_cast<float>(Window::GAME_HEIGHT));
    Texture2d::gameCamera.setMinWidth(100);

    if (editMode) {
        float gx = Texture2d::gameCamera.x;
        float gy = Texture2d::gameCamera.y;
        float gw = Texture2d::gameCamera.w;
        float gh = Texture2d::gameCamera.h;
        Texture2d::gameCamera.reset();
        Texture2d::gameCamera.expandToInclude(0, 0);
        Texture2d::gameCamera.expandToInclude(Window::GAME_WIDTH, Window::GAME_HEIGHT);
        Texture2d::setColor(vec4(1, 1, 0, 0.25f));
        platform.draw(gx, gy, gw, gh);
    }

    //background
    Texture2d::setColor(vec4(0.0f, 0.1f, 0.2f, 1.0f));
    platform.drawRaw(window->gx, window->gy, window->gw, window->gh);

    //draw platforms
    int platCounter = 0;
    for (auto &plat: Player::levels[Player::currentLevel].platforms) {
        int pc = 0;
        vec4 color = vec4(1);
        //count how many players are on current platform
        for (auto &player: Player::players) if (player.platStandingOn == platCounter) pc++;
        if (pc > 0) { //if its more then 0, add their colors together to make the platform color
            color = vec4(0);
            for (int i = 0; i < Player::MAX_PLAYERS; i++) if (platCounter == Player::players[i].platStandingOn) color += Player::playerColors[i] / static_cast<float>(pc);
        }

        plat.draw(color);
        platCounter++;
    }

    std::string connectedPlayers;
    //players
    for (int i = 0; i < Player::MAX_PLAYERS; i++) {
        connectedPlayers += Player::players[i].connected ? "1" : "0";
        if (!Player::players[i].connected) continue;
        //handle input
        if (i == Client::id && !camera.lock && !chatBox.open) {
            bool mouseLeft = glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT);
            bool left = glfwGetKey(window->window, GLFW_KEY_A);
            bool jump = glfwGetKey(window->window, GLFW_KEY_SPACE) || glfwGetKey(window->window, GLFW_KEY_W);
            bool down = glfwGetKey(window->window, GLFW_KEY_S) || glfwGetKey(window->window, GLFW_KEY_LEFT_CONTROL);
            bool right = glfwGetKey(window->window, GLFW_KEY_D);
            float mx = window->gX(window->mousePos.x);
            float my = window->gY(window->getHeight() - window->mousePos.y);

            if (Player::players[i].mousePos.x != mx) Player::players[i].mousePos.x = Client::updateMousePos(true, mx);
            if (Player::players[i].mousePos.y != my) Player::players[i].mousePos.y = Client::updateMousePos(false, my);
            if (Player::players[i].keys[MOUSE_LEFT] != mouseLeft) Player::players[i].keys[MOUSE_LEFT] = Client::updateKeyPress(MOUSE_LEFT, mouseLeft);
            if (Player::players[i].keys[KEY_LEFT] != left) Player::players[i].keys[KEY_LEFT] = Client::updateKeyPress(KEY_LEFT, left);
            if (Player::players[i].keys[KEY_JUMP] != jump) Player::players[i].keys[KEY_JUMP] = Client::updateKeyPress(KEY_JUMP, jump);
            if (Player::players[i].keys[KEY_DOWN] != down) Player::players[i].keys[KEY_DOWN] = Client::updateKeyPress(KEY_DOWN, down);
            if (Player::players[i].keys[KEY_RIGHT] != right) Player::players[i].keys[KEY_RIGHT] = Client::updateKeyPress(KEY_RIGHT, right);
        }



        //draw players
        Player::players[i].draw();
    }


    if (camera.lock && !editMode) {
        Texture2d::setColor(vec4(0, 0, 0, 0.5f));
        platform.draw(0, 0, Window::GAME_WIDTH, Window::GAME_HEIGHT);
    }




    //font setup
    fontRenderer.setColor(Player::playerColors[Client::id]);
    float fontScale = 3.0f;
    float mx = window->gX(window->mousePos.x);
    float my = window->gY(window->getHeight() - window->mousePos.y);

    //click IP to copy it
    float ipx1 = 10;
    float ipx2 = ipx1 + fontScale * fontRenderer.getWidth("ip: " + Client::ip);
    float ipy1 = Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10 - (4.0f * (fontScale * (fontRenderer.getHeight() + (2.5f * fontRenderer.characterSpacing))));
    float ipy2 = ipy1 + fontScale * fontRenderer.getHeight();
    if (mx >= ipx1 - 5 && mx <= ipx2 + 5 && my >= ipy1 - 5 && my <= ipy2 + 5) {
        Texture2d::setColor(mouseDown ? vec4(0, 0, 0, 0.4f) : vec4(0, 0, 0, 0.2f));
        platform.draw(ipx1 - 5, ipy1 - 5, ipx2 - ipx1 + 10, ipy2 - ipy1 + 10);
        if (mousePressed) glfwSetClipboardString(window->window, Client::ip.c_str());
    }

    //draw misc text
    fontRenderer.draw(
            "x: " + std::to_string(camera.lock ? (int) mx : (int) Player::players[Client::id].pos.x) +
            "\ny: " + std::to_string(camera.lock ? (int) my : (int) Player::players[Client::id].pos.y) +
            "\nxVel: " + std::to_string((int) Player::players[Client::id].velocity.x) +
            "\nyVel: " + std::to_string((int) Player::players[Client::id].velocity.y) +
            "\nip: " + Client::ip, 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);

    //debug mode data
    std::string debugString;
    if (Client::debug) {
        debugString.append("\n%1Debug mode enabled\nid: " + std::to_string(Client::id) +
                           "\n[" + connectedPlayers + "]" +
                           "\nHost: " + (Client::hostClient ? "true" : "false") + "\n");
    }
    if (Client::id == Player::MAX_PLAYERS) {
        debugString.append("\n%3Controls\nFullscreen: F11\nDebug: F12\nBuild Mode: F8\nMovement: A, D, LCTRL/S, and W/Space\nOpen Chat: T\n\n%4Commands:\n/host\n/join [ip]\n/leave\n/ping\n");
    }

    //edit mode
    if (editMode) {
        //edit mode place platform
        Platform newPlatform = Platform(0, 0, 0, 0);
        if (mouseDownStart.x != -1) { //actually drawing plat
            if (mouseDown || mouseReleased) {
                float npx = window->gX(mouseDownStart.x); //new platform x
                float npy = window->gY(window->getHeight() - mouseDownStart.y); //new platform y
                newPlatform.x = npx;
                newPlatform.y = npy;
                newPlatform.w = mx - npx;
                newPlatform.h = my - npy;
                if (newPlatform.w < 0) {
                    newPlatform.w *= -1;
                    newPlatform.x -= newPlatform.w;
                }
                if (newPlatform.h < 0) {
                    newPlatform.h *= -1;
                    newPlatform.y -= newPlatform.h;
                }
                newPlatform.solid = true;

                if (newPlatform.w > 0 && newPlatform.h > 0) {
                    newPlatform.draw(Player::playerColors[Player::MAX_PLAYERS] * 0.5f);
                }
                if (mouseReleased) {
                    Player::levels[Player::currentLevel].platforms.push_back(newPlatform);
                }
            } else if (rightMouseDown || rightMouseReleased) {
                float npx = window->gX(mouseDownStart.x); //new platform x
                float npy = window->gY(window->getHeight() - mouseDownStart.y); //new platform y
                newPlatform.x = npx;
                newPlatform.y = npy;
                newPlatform.w = mx - npx;
                newPlatform.h = my - npy;
                if (newPlatform.w < 0) {
                    newPlatform.w *= -1;
                    newPlatform.x -= newPlatform.w;
                }

                if (newPlatform.h != 0 && abs(newPlatform.h) != 5) newPlatform.h = 5 * (newPlatform.h / abs(newPlatform.h));

                if (newPlatform.h < 0) {
                    newPlatform.h *= -1;
                    newPlatform.y -= newPlatform.h;
                }


                newPlatform.solid = false;

                if (newPlatform.w > 0 && newPlatform.h > 0) {
                    newPlatform.draw(Player::playerColors[Player::MAX_PLAYERS] * 0.5f);
                }
                if (rightMouseReleased) {
                    Player::levels[Player::currentLevel].platforms.push_back(newPlatform);
                }
            }

        }

        //edit mode data
        int platHovering = -1;

        for (int i = 0; i < Player::levels[Player::currentLevel].platforms.size(); i++) {
            Platform *p = &Player::levels[Player::currentLevel].platforms[i];
            if (mx >= p->x && mx <= p->x + p->w && my >= p->y && my <= p->y + p->h) {
                platHovering = i;
                p->draw(vec4(Player::playerColors[Client::id]));
                if (glfwGetKey(window->window, GLFW_KEY_BACKSPACE)) {
                    Player::levels[Player::currentLevel].platforms.erase(Player::levels[Player::currentLevel].platforms.begin() + i);
                }
                break;
            }
        }
        debugString.append("\n%" + std::to_string(Player::MAX_PLAYERS) + "Edit mode enabled\nPlatform ID: " + (mouseDownStart.x == -1 ? (platHovering == -1 ? "None" : std::to_string(platHovering)) : "New") +
                           "\nPlatform Data: " + (mouseDownStart.x == -1 ? (platHovering != -1 ? Player::levels[Player::currentLevel].platforms[platHovering].to_string() : "None") : newPlatform.to_string()));
    }

    if (!debugString.empty()) {
        fontRenderer.setColor(Player::playerColors[(Client::id + 1) % Player::MAX_PLAYERS]);
        fontRenderer.draw(debugString, ipx1, ipy1 - (fontScale * (fontRenderer.getHeight() + (2.5f * fontRenderer.characterSpacing))), 3.0f);
    }

    //render chat
    if (!Client::history.empty()) {
        Texture2d::setColor(vec4(0.05f, 0.05f, 0.1f, chatBox.open ? 0.8f : 0.4f));
        platform.draw(0, 0, (chatBox.getWidth() + 8) * chatBox.scale, ((fontRenderer.getHeight() + (2.5f * fontRenderer.characterSpacing)) * static_cast<float>(Client::history.size() + 1) + 7) * chatBox.scale);
    }

    chatBox.draw();


    if (mouseReleased || rightMouseReleased) mouseDownStart = vec2(-1);
}

void GameScene::keyPress(int key, int action, int mods) {
    camera.handleKeyboard(key, action);
    if (action == GLFW_PRESS) {
        if (chatBox.handleKeyboard(key, mods) == RETURN) return;
        if (key == GLFW_KEY_ESCAPE && !editMode) {
            if (camera.lock) {
                glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
                glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
                //glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        } else if (key == GLFW_KEY_P) {
            std::cout << camera.getString() << std::endl;
        } else if (key == GLFW_KEY_F12) {
            Client::debug = !Client::debug;
        } else if (key == GLFW_KEY_F8) {
            editMode = !editMode;
            if (editMode) {
                camera.lock = true;
                glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                camera.lock = false;
                //glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        } else if (key == GLFW_KEY_F11) {
            borderless = !borderless;
            if (borderless) {
                GLFWmonitor *monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode *mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                Window::setVsync(true);
            } else {
                glfwSetWindowMonitor(window->window, nullptr, 100, 100, Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT, GLFW_DONT_CARE);
                Window::setVsync(true);
            }
        }
    }
}

void GameScene::mouseMove(float x, float y) {
    camera.handleMouse(x, y);
    window->mousePos.x = x;
    window->mousePos.y = y;
}


