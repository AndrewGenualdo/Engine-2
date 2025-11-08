//
// Created by cobble on 11/3/2025.
//

#ifndef ENGINE_2_WORLD_H
#define ENGINE_2_WORLD_H
#include <string>
#include <bitset>
#include <vector>

#include "FarmingObject.h"
#include "cobb/misc/fontRenderer.h"
#include "glm/vec2.hpp"
#include "cobb/misc/texture2d.h"
#include "cobb/misc/tiles2d.h"
#include "items/produce/ItemProduceCarrot.h"
#include "items/produce/ItemProduceTomato.h"
#include "items/seeds/ItemSeedCarrot.h"
#include "items/seeds/ItemSeedTomato.h"
#include "tiles/plants/TilePlantCarrot.h"
#include "tiles/plants/TilePlantTomato.h"

using namespace cobb;
using namespace glm;

class LittleGuy;

class FarmingWorld {
public:
    constexpr static int EMPTY = 0;
    constexpr static int FARMLAND = 1;
    constexpr static int FARMLAND_TEXTURE_OFFSET = 16;
    constexpr static int MUD = 2;
    constexpr static int GRASS = 3;
    constexpr static int WATER = 4;
    constexpr static int STONE_PATH = 5;
    constexpr static int STONE_PATH_2 = 6;

    constexpr static int TILE_OFFSET_X = 55;
    constexpr static int TILE_OFFSET_Y = 65;
    constexpr static int TILE_WIDTH = 75;
    constexpr static int TILE_HEIGHT = 75;

    constexpr static int TEXTURE_TILE_COUNT = 16;

    constexpr static int TILES_HORIZ = 24;
    constexpr static int TILES_VERT = 13;

    constexpr static int TICKS_PER_SECOND = 10;

    constexpr static ivec2 INVENTORY_TILE = ivec2(0,1);

    unsigned int landData[TILES_HORIZ * TILES_VERT];
    unsigned int plantData[TILES_HORIZ * TILES_VERT];

    static Tiles2d landTilemap;
    static Tiles2d plantTilemap;
    static Texture2d barnTexture;
    static MultiTexture2d uiTexture;
    static FontRenderer *fontRenderer;

    std::string savePath;

    std::vector<LittleGuy*> guys;
    std::vector<FarmingObject*> objects;
    std::map<FarmingObject::TypeID, int> inventory;

    FarmingWorld();
    explicit FarmingWorld(const std::string &savePath);
    ~FarmingWorld();

    void load();
    void save() const;
    void cleanup();

    void update(float dt);
    void tick();
    void draw() const;

    void updateTileTypes();
    void clearObjects();

    static bool isFarmland(int texIndex);
    [[nodiscard]] bool isFarmland(int x, int y) const;

    [[nodiscard]] static vec2 getTilePos(int x, int y);
    [[nodiscard]] static ivec2 getTileFromPos(vec2 pos);

    static Item* createItem(FarmingObject::TypeID type) {
        switch (type) {
            case FarmingObject::TypeID::ITEM_PRODUCE_TOMATO: return new ItemProduceTomato();
            case FarmingObject::TypeID::ITEM_PRODUCE_CARROT: return new ItemProduceCarrot();
            case FarmingObject::TypeID::ITEM_SEED_TOMATO: return new ItemSeedTomato();
            case FarmingObject::TypeID::ITEM_SEED_CARROT: return new ItemSeedCarrot();
            default: return nullptr;
        }
    }

    Item* createItem(FarmingObject::TypeID type, ivec2 tile) {
        return createItem(type, getTilePos(tile.x, tile.y));
    }
    Item* createItem(FarmingObject::TypeID type, vec2 pos) {
        Item *item = createItem(type);
        if (item != nullptr) {
            item->pos = pos;
            objects.push_back(item);
        }
        return item;
    }

    static Tile* createTile(FarmingObject::TypeID type, ivec2 tile) {
        switch (type) {
            case FarmingObject::TypeID::TILE_PLANT_TOMATO: return new TilePlantTomato(tile);
            case FarmingObject::TypeID::TILE_PLANT_CARROT: return new TilePlantCarrot(tile);
            default: return nullptr;
        }
    }

};


#endif //ENGINE_2_WORLD_H