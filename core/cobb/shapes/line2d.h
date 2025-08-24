//
// Created by cobble on 7/10/2025.
//

#ifndef LINE2D_H
#define LINE2D_H

#include "../core/camera.h"
#include "../core/shader.h"
#include "../misc/texture2d.h"
#include "glm/detail/type_vec3.hpp"
#include "glm/detail/type_vec4.hpp"

using namespace glm;

namespace cobb {

    static unsigned int LINE_2D_VAO = -1, LINE_2D_VBO = -1;

    constexpr static float VERTICES_LINE_2D[] = {
        0, 0,
        1, 0,
    };



    class Line2d {
    public:

        static Shader *line2dShader;

        static unsigned int* getVAO();
        static unsigned int* getVBO();

        vec2 _start;
        vec2 _end;
        vec4 _color;

        Line2d() {
            _start = vec2(0);
            _end = vec2(1);
            _color = vec4(1);
            load();
        }

        Line2d(vec2 start, vec2 end, vec4 color) {
            _start = start;
            _end = end;
            _color = color;
            load();
        }

        void load() {
            loadShader();
            line2dShader->use();

            glBindVertexArray(*getVAO());

            glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
            glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_LINE_2D), VERTICES_LINE_2D, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        virtual void draw(bool bind = false) {
            //translates from world coordinates to game camera coordinates
            Texture2d::gameCamera.adjustToAspectRatio(Window::GAME_WIDTH / Window::GAME_HEIGHT);
            Texture2d::gameCamera.setMinWidth(100);

            float gsw = Window::GAME_WIDTH / Texture2d::gameCamera.w;
            float gsh = Window::GAME_HEIGHT / Texture2d::gameCamera.h;

            float gx1 = (_start.x - Texture2d::gameCamera.x) * gsw;
            float gy1 = (_start.y - Texture2d::gameCamera.y) * gsh;
            float nx1 = Texture2d::window->sX(gx1) + Texture2d::window->sX(0) * (gx1 / Window::GAME_WIDTH);
            float ny1 = Texture2d::window->sY(gy1) + Texture2d::window->sY(0) * (gy1 / Window::GAME_HEIGHT);

            float gx2 = (_end.x - Texture2d::gameCamera.x) * gsw;
            float gy2 = (_end.y - Texture2d::gameCamera.y) * gsh;
            float nx2 = Texture2d::window->sX(gx2) + Texture2d::window->sX(0) * (gx2 / Window::GAME_WIDTH);
            float ny2 = Texture2d::window->sY(gy2) + Texture2d::window->sY(0) * (gy2 / Window::GAME_HEIGHT);

            drawRaw(vec2(nx1, ny1), vec2(nx2, ny2), bind);
        }

        virtual void drawRaw(vec2 start, vec2 end, bool bind = false) {
            loadShader();
            if(bind) {
                line2dShader->use();
                glBindVertexArray(*getVAO());
            }

            vec2 direction = end - start;
            float length = glm::length(direction);
            float angle = atan2(direction.y, direction.x);

            mat4 model = mat4(1);
            model = translate(model, vec3(start, 0.0f));
            model = rotate(model, angle, vec3(0, 0, 1));
            model = scale(model, vec3(length, 1, 1));
            line2dShader->setMat4("model", model);
            line2dShader->setVec4("color", _color);
            glDrawArrays(GL_LINES, 0, 2);
        }

        static void loadShader() {
            if(line2dShader == nullptr) {
                std::cout << "Loaded line2d shader!" << std::endl;
                line2dShader = new Shader("assets/line2d");
                line2dShader->use();
                glBindVertexArray(*getVAO());
            }
        }
    };
}


#endif //LINE2D_H
