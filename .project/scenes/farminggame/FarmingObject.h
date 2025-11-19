//
// Created by cobble on 11/5/2025.
//

#ifndef ENGINE_2_FARMINGOBJECT_H
#define ENGINE_2_FARMINGOBJECT_H

#include "cobb/misc/multiTexture2d.h"
#include <sstream>

class FarmingWorld;
using namespace glm;
using namespace cobb;

class FarmingObject {

public:
    enum class TypeID {
        NONE = 0,


        ITEM,

        ITEM_SEED,
        ITEM_SEED_TOMATO,
        ITEM_SEED_CARROT,
        ITEM_SEED_BLUEBERRY,


        ITEM_PRODUCE,
        ITEM_PRODUCE_TOMATO,
        ITEM_PRODUCE_CARROT,
        ITEM_PRODUCE_BLUEBERRY,


        TILE,

        TILE_PLANT,
        TILE_PLANT_TOMATO,
        TILE_PLANT_CARROT,
        TILE_PLANT_BLUEBERRY,


        LITTLE_GUY
    };

    class ObjectData {
    public:
        std::string configKey;
        TypeID type = TypeID::NONE;
        TypeID parent = TypeID::NONE;
        ObjectData() = default;
        explicit ObjectData(const std::string &configKey, TypeID type, TypeID parent) {
            this->configKey = configKey;
            this->type = type;
            this->parent = parent;
        }
        virtual ~ObjectData();
    };

    static std::map<TypeID, ObjectData*> objectData;

    static void loadData();
    static void cleanData();
    static FarmingWorld *world;
    static void setWorld(FarmingWorld *world);
    static void loadInventory();

    ivec2 tile = ivec2(0);
    bool beingUsed = false;

    FarmingObject() = default;
    explicit FarmingObject(ivec2 tile);
    virtual ~FarmingObject();

    virtual void update(float deltaTime);
    virtual void tick();
    virtual void draw(bool bind);
    virtual void draw(float offsetX, float offsetY, bool bind);
    [[nodiscard]] virtual TypeID getType() const;
    virtual bool destroy();

    void setBeingUsed(bool isBeingUsed);
    [[nodiscard]] bool isBeingUsed() const;

    virtual std::string getConfig();
    virtual void loadConfig(const std::string &line, int i);

    template <typename T>
    static T* getData(TypeID type) {
        if (objectData.find(type) != objectData.end()) {
            T* out = dynamic_cast<T*>(objectData[type]);
            if (out) return out;

            std::cout << "FAILED TO CAST TO OBJECT TYPE: " << objectData[type]->configKey << std::endl;
            return nullptr;
        }

        std::cout << "FAILED TO FIND OBJECT TYPE: " << objectData[type]->configKey << std::endl;
        return nullptr;
    }

    static bool isType(TypeID type) {


        return false;
    }
};



#endif //ENGINE_2_FARMINGOBJECT_H
