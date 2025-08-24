//
// Created by cobble on 12/27/2024.
//

#ifndef ENGINE_2_ANIMATION_H
#define ENGINE_2_ANIMATION_H

#include "multiTexture2d.h"

using namespace cobb;
using namespace glm;

class Animation {
public:
    MultiTexture2d texture;
    float delay;
    bool loop;
    float startTime;


    Animation() = default;
    Animation(const std::string& path, int frames, float delay, bool loop) {
        this->texture = MultiTexture2d(path, frames);
        this->delay = delay;
        this->loop = loop;
        this->startTime = 0.0f;
    }

    void bind() {
        texture.bind();
    }

    [[nodiscard]] int bindSingle(float time, bool start) {
        if(start) startTime = time;
        float t = time - startTime;
        float totalLength = static_cast<float>(texture.getFrames()) * delay;
        int frame = (int) (static_cast<float>(texture.getFrames()) * t / totalLength);
        if(frame >= texture.getFrames()) frame = texture.getFrames() - 1;
        texture.bind();
        return frame;
    }

    [[nodiscard]] int bindLoop(float time) {
        float totalLength = static_cast<float>(texture.getFrames()) * delay;
        int cycle = static_cast<int>(time / totalLength);
        float t = time - totalLength * static_cast<float>(cycle);
        int frame = static_cast<int>(static_cast<float>(texture.getFrames()) * t / totalLength);
        texture.bind();
        return frame;
    }

    void drawSingle(float time, float x, float y, float width, float height, bool start, bool shouldBind = true) {
        int frame = bindSingle(time, start);
        if(shouldBind) texture.bind();
        texture.draw(x, y, width, height, frame, false);
    }

    void drawLoop(float time, float x, float y, float width, float height, bool shouldBind = true) {
        int frame = bindLoop(time);
        if(shouldBind) texture.bind();
        texture.draw(x, y, width, height, frame, false);
    }

    void drawSingle(float time, float x, float y, float width, float height, bool start, float rotation, bool shouldBind = true) {
        int frame = bindSingle(time, start);
        if(shouldBind) texture.bind();
        texture.draw(x, y, width, height, rotation, frame, false);
    }

    void drawLoop(float time, float x, float y, float width, float height, float rotation, bool shouldBind = true) {
        int frame = bindLoop(time);
        if(shouldBind) texture.bind();
        texture.draw(x, y, width, height, rotation, frame, false);
    }


};


#endif //ENGINE_2_ANIMATION_H
