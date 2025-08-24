//
// Created by cobble on 1/10/2025.
//

#ifndef ENGINE_2_MULTITEXTURE2D_H
#define ENGINE_2_MULTITEXTURE2D_H


#include "texture2d.h"

using namespace cobb;

static Shader *multiTexture2dShader;

class MultiTexture2d : public Texture2d {

    int m_frames;


public:

    MultiTexture2d();
    MultiTexture2d(const std::string &path, int frames);

    [[nodiscard]] float getWidth() const override;
    [[nodiscard]] float getTotalWidth() const;
    [[nodiscard]] int getFrames() const;

    void draw(float x, float y, float width, float height, int frame, bool shouldBind = true) const;
    void draw(float x, float y, float width, float height, float rotation, int frame, bool shouldBind = true) const;
    void drawRaw(float x, float y, float width, float height, int frame, bool shouldBind = true) const;
    void drawRaw(float x, float y, float width, float height, float rotation, int frame, bool shouldBind = true) const;

    static void loadShader();



};


#endif //ENGINE_2_MULTITEXTURE2D_H
