//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_OBJECT_H
#define ENGINE_2_OBJECT_H

#include "cobb/misc/multiTexture2d.h"
#include <sstream>

using namespace glm;
using namespace cobb;

class FarmingObject {

public:
    enum ObjectType {
        NONE,
        ITEM,
        TILE,
        MISC
    };

    class ObjectData {
    public:
        ObjectType objectType;

        ObjectData() = default;
        explicit ObjectData(const ObjectType objectType) {
            this->objectType = objectType;
        }
    };

    static std::vector<ObjectData*> objectData;

    static void loadData();

    ivec2 tile = ivec2(0);
    ObjectType type = NONE;
    bool beingUsed = false;

    FarmingObject() = default;
    FarmingObject(ObjectType type, ivec2 tile);
    virtual ~FarmingObject() = default;

    virtual void update(float deltaTime);
    virtual void tick();
    virtual void draw(bool bind);
    virtual void draw(float offsetX, float offsetY, bool bind);

    void setUsed(bool isBeingUsed);
    [[nodiscard]] bool isBeingUsed() const;

    virtual std::string getConfigKey();
    virtual std::string getConfig();
    virtual void loadConfig(const std::string &line, int i);

};


#endif //ENGINE_2_OBJECT_H