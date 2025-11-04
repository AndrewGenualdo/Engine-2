//
// Created by cobble on 11/3/2025.
//

#ifndef ENGINE_2_TILES2D_H
#define ENGINE_2_TILES2D_H

#include "texture2d.h"

using namespace cobb;

static Shader *tiles2dShader;

class Tiles2d : public Texture2d {

    int m_tilesHoriz, m_tilesVert, m_textureTileCount;
    unsigned int *m_data;

public:
    Tiles2d();
    Tiles2d(const std::string &path, int width, int height, int textureTileCount, unsigned int *data);

    //[[nodiscard]] float getWidth() const override;
    //[[nodiscard]] float getTotalWidth() const;
    //[[nodiscard]] int getFrames() const;

    /*void draw(float x, float y, float width, float height, int frame, bool shouldBind = true) const;
    void draw(float x, float y, float width, float height, float rotation, int frame, bool shouldBind = true) const;
    void drawRaw(float x, float y, float width, float height, int frame, bool shouldBind = true) const;
    void drawRaw(float x, float y, float width, float height, float rotation, int frame, bool shouldBind = true) const;*/
    void draw(float x, float y, float width, float height, bool shouldBind = true) const;
    void drawRaw(float x, float y, float width, float height, bool shouldBind = true) const;

    static void loadShader();

};


#endif //ENGINE_2_TILES2D_H