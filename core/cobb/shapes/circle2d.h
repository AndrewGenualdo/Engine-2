//
// Created by cobble on 7/11/2025.
//

#ifndef CIRCLE2D_H
#define CIRCLE2D_H



#include "../core/camera.h"
#include "../core/shader.h"
#include "../misc/texture2d.h"
#include "glm/detail/type_vec3.hpp"
#include "glm/detail/type_vec4.hpp"

using namespace glm;

namespace cobb {

    static unsigned int CIRCLE_2D_VAO = -1, CIRCLE_2D_VBO = -1, CIRCLE_2D_EBO = -1;

    constexpr static float VERTICES_CIRCLE_2D[] = {
        // full-screen quad (in normalized device coords [-1, 1])
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    constexpr static unsigned int INDICES_CIRCLE_2D[] = {
        0, 1, 2,
        2, 3, 0
    };



    class Circle2d {
    public:

        static Shader *circle2dShader;

        static unsigned int* getVAO();
        static unsigned int* getVBO();
        static unsigned int* getEBO();

        vec2 _pos;
        float _radius;
        float _thickness;
        vec4 _color;
        float _glow;

        Circle2d() {
            _pos = vec2(0);
            _radius = 1;
            _thickness = 0.015;
            _color = vec4(1);
            _glow = 0.01;
            load();
        }

        Circle2d(vec2 pos, float radius, float thickness, vec4 color) {
            _pos = pos;
            _radius = radius;
            _thickness = thickness * 0.5f;
            _color = color;
            _glow = 0.01;
            load();
        }

        void setGlow(float glow) {
            _glow = glow * 0.5f;
        }

        vec2 pos() {
            return _pos;
        }

        void setPos(vec2 pos) {
            _pos = pos;
        }

        float size() {
            return _radius;
        }



        void load() {
            loadShader();
            circle2dShader->use();

            glBindVertexArray(*getVAO());

            glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
            glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_CIRCLE_2D), VERTICES_CIRCLE_2D, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *getEBO());
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES_CIRCLE_2D), INDICES_CIRCLE_2D, GL_STATIC_DRAW);

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

            float gr = _radius * gsw;
            float nr = Texture2d::window->sX(gr);

            drawRaw(vec2(nx, ny), nr, bind);
        }

        virtual void drawRaw(vec2 pos, float radius, bool bind = false) {
            loadShader();
            if(bind) {
                circle2dShader->use();
                glBindVertexArray(*getVAO());
            }

            mat4 model = mat4(1.0f);
            model = translate(model, vec3(pos, 0.0f));
            float totalRadius = radius + _glow; // glow is a uniform you pass in
            model = scale(model, vec3(totalRadius * 2.0f, totalRadius * 2.0f, 1.0f));

            circle2dShader->setMat4("model", /*Object::translate(pos.x + _radius * 0.5f, pos.y + _radius, 0) * Object::scale(_radius * 2, _radius * 2, 1)*/model);
            circle2dShader->setVec4("color", _color);
            circle2dShader->setFloat("thickness", _thickness);
            circle2dShader->setFloat("glow", _glow);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        static void loadShader() {
            if(circle2dShader == nullptr) {
                std::cout << "Loaded circle2d shader!" << std::endl;
                circle2dShader = new Shader("assets/circle2d");
                circle2dShader->use();
                glBindVertexArray(*getVAO());
            }
        }
    };
}



#endif //CIRCLE2D_H
