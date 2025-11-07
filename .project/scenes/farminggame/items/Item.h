//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_ITEM_H
#define ENGINE_2_ITEM_H

#include "../FarmingObject.h"

using namespace glm;
using namespace cobb;

class Item : public FarmingObject {

    static MultiTexture2d itemsTexture;
    static int width, height;

public:

    static void setTexture(const std::string &path, int width, int height, int items);

    class ItemData : public ObjectData {
    public:

        int textureIndex = 63;

        ItemData() : ObjectData() {}
        explicit ItemData(const TypeID type, const TypeID parent, const int textureIndex) : ObjectData(type, parent) {
            this->textureIndex = textureIndex;
        }
        ~ItemData() override = default;
    };




    vec2 pos = vec2(0);


    Item() = default;
    explicit Item(ivec2 tile);
    explicit Item(vec2 pos);
    ~Item() override = default;
    [[nodiscard]] TypeID getType() const override;

    void update(float dt) override;
    void tick() override;
    void draw(bool bind) override;
    void draw(float offsetX, float offsetY, bool bind) override;

    std::string getConfigKey() override;
    std::string getConfig() override;
    void loadConfig(const std::string &line, int i) override;

};


#endif //ENGINE_2_ITEM_H