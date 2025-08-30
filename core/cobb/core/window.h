//
// Created by andrew.genualdo on 9/17/2024.
//


#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <string>
#include "../../ew/external/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include <iostream>
#include <map>
#include <vector>


namespace cobb {


    class Window {
    public:
        static constexpr float SCREEN_WIDTH = 1280;//1920;
        static constexpr float SCREEN_HEIGHT = 720;//1080;

        static constexpr float GAME_WIDTH = 1920;
        static constexpr float GAME_HEIGHT = 1080;
        //static constexpr float GAME_WIDTH = 500;
        //static constexpr float GAME_HEIGHT = 1440;

        GLFWwindow *window;

        glm::vec2 mousePos;

        Window();

        explicit Window(const std::string &title);

        void pollInputs();

        bool isInputClicked(int input);
        bool isInputPressed(int input);
        bool isInputReleased(int input);

        float update();

        [[nodiscard]] float getTime() const;

        [[nodiscard]] float getWidth() const;

        [[nodiscard]] float getHeight() const;

        /*
        * Converts screen x coordinates into game x coordinates
        */
        [[nodiscard]] float gX(float screenX) const;

        /*
         * Converts screen y coordinates into game y coordinates
        */
        [[nodiscard]] float gY(float screenY) const;

        /*
        * Converts game x coordinates into screen x coordinates
        */
        [[nodiscard]] float sX(float x) const;

        /*
        * Converts game y coordinates into screen y coordinates
        */
        [[nodiscard]] float sY(float y) const;

        void getGameDimensions();

        void setWidth(int width);

        void setHeight(int height);

        void setTime(double time);

        static void setVsync(bool vsync);

        void setWindowTitle(const std::string& title);

    private:
        std::string _title;
        int _uptime;
        int _frames;
        float _time;
        int _width; //screen width
        int _height; //screen height
        double _timeOffset;
    public:
        float gx, gy, gw, gh; //game x, game y, game width, game height


        std::vector<int> inputs = {
            GLFW_MOUSE_BUTTON_LEFT,
            GLFW_MOUSE_BUTTON_MIDDLE,
            GLFW_MOUSE_BUTTON_RIGHT,
            GLFW_KEY_A,
            GLFW_KEY_B,
            GLFW_KEY_C,
            GLFW_KEY_D,
            GLFW_KEY_E,
            GLFW_KEY_F,
            GLFW_KEY_G,
            GLFW_KEY_H,
            GLFW_KEY_I,
            GLFW_KEY_J,
            GLFW_KEY_K,
            GLFW_KEY_L,
            GLFW_KEY_M,
            GLFW_KEY_N,
            GLFW_KEY_O,
            GLFW_KEY_P,
            GLFW_KEY_Q,
            GLFW_KEY_R,
            GLFW_KEY_S,
            GLFW_KEY_T,
            GLFW_KEY_U,
            GLFW_KEY_V,
            GLFW_KEY_W,
            GLFW_KEY_X,
            GLFW_KEY_Y,
            GLFW_KEY_Z,
            GLFW_KEY_SPACE,
            GLFW_KEY_LEFT_CONTROL
        };

        std::map<int, bool> wasInputDown = std::map<int, bool>();
        std::map<int, bool> isInputDown = std::map<int, bool>();
    };


}


#endif //WINDOW_H
