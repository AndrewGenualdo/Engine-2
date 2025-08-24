//
// Created by cobble on 8/18/2025.
//

#ifndef SPLINE_H
#define SPLINE_H

#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include "../core/shader.h"
#include "../misc/texture2d.h"

using namespace glm;

namespace cobb {
    static unsigned int SPLINE_2D_VAO = -1, SPLINE_2D_VBO = -1, SPLINE_2D_EBO = -1;

    constexpr static float VERTICES_SPLINE_2D[] = {
        // full-screen quad (in normalized device coords [-1, 1])
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    const unsigned int INDICES_SPLINE_2D[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    static Shader *spline2dShader;


    class Spline2d {
    public:

        static unsigned int* getVAO();
        static unsigned int* getVBO();
        static unsigned int* getEBO();

        vec2 _pos;
        float _thickness;
        vec4 _color;
        float _glow;

        Spline2d() {
            load();
        }

        void load() {
            loadShader();
            spline2dShader->use();

            glBindVertexArray(*getVAO());

            glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
            glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_SPLINE_2D), VERTICES_SPLINE_2D, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *getEBO());
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES_SPLINE_2D), INDICES_SPLINE_2D, GL_STATIC_DRAW);

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

            float gx = (_pos.x - Texture2d::gameCamera.x) * gsw;
            float gy = (_pos.y - Texture2d::gameCamera.y) * gsh;
            float nx = Texture2d::window->sX(gx) + Texture2d::window->sX(0) * (gx / Window::GAME_WIDTH);
            float ny = Texture2d::window->sY(gy) + Texture2d::window->sY(0) * (gy / Window::GAME_HEIGHT);

            drawRaw(vec2(nx, ny), bind);
        }

        virtual void drawRaw(vec2 pos, bool bind = false) {
            loadShader();
            if(bind) {
                spline2dShader->use();
                glBindVertexArray(*getVAO());
            }

            mat4 model = mat4(1.0f);
            model = translate(model, vec3(pos, 0.0f));

            spline2dShader->setMat4("model", model);
            spline2dShader->setVec4("color", _color);
            spline2dShader->setFloat("thickness", _thickness);
            spline2dShader->setFloat("glow", _glow);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        static void loadShader() {
            if(spline2dShader == nullptr) {
                std::cout << "Loaded spline2d shader!" << std::endl;
                spline2dShader = new Shader("assets/spline2d");
                spline2dShader->use();
                glBindVertexArray(*getVAO());
            }
        }
    };
}




#endif //SPLINE_H
