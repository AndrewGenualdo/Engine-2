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
#include "items/misc/ItemGold.h"
#include "items/produce/ItemProduceBlueberry.h"
#include "items/produce/ItemProduceCarrot.h"
#include "items/produce/ItemProduceTomato.h"
#include "items/seeds/ItemSeedBlueberry.h"
#include "items/seeds/ItemSeedCarrot.h"
#include "items/seeds/ItemSeedTomato.h"
#include "misc/Truck.h"
#include "tiles/plants/TilePlantBlueberry.h"
#include "tiles/plants/TilePlantCarrot.h"
#include "tiles/plants/TilePlantTomato.h"

using namespace cobb;
using namespace glm;

class LittleGuyManager;

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
    constexpr static ivec2 STAND_TILE = ivec2(10, 0);

    unsigned int landData[TILES_HORIZ * TILES_VERT];
    unsigned int plantData[TILES_HORIZ * TILES_VERT];

    static Tiles2d landTilemap;
    static Tiles2d plantTilemap;
    static Texture2d barnTexture;
    static Texture2d standTexture;

    static MultiTexture2d uiTexture;
    static FontRenderer *fontRenderer;

    std::string savePath;

private:
    std::vector<Tile*> tiles = std::vector<Tile*>();
public:
    std::vector<Item*> items;
    std::map<FarmingObject::TypeID, int> inventory;
    std::map<FarmingObject::TypeID, int> effectiveInventory;

    LittleGuyManager *guyManager = nullptr;
    Truck truck;
    bool logs = false;
    float time = 0.0f;

    FarmingWorld();
    explicit FarmingWorld(const std::string &savePath);
    ~FarmingWorld();

    void load();
    void save() const;
    void cleanup();

    void update(float dt);
    void tick(bool isTick, float dt);
    void draw() const;

    void updateTileTypes();
    void clear();

    void resetEffectiveInventory(); //should be called when resetting tasks

    static bool isFarmland(int texIndex);
    [[nodiscard]] bool isFarmland(int x, int y) const;

    [[nodiscard]] static vec2 getTilePos(int x, int y);
    [[nodiscard]] static ivec2 getTileFromPos(vec2 pos);

    [[nodiscard]] std::vector<ivec2> getFarmland(int amount) const;
    [[nodiscard]] std::vector<ivec2> getTiles(FarmingObject::TypeID type, int amount) const;

    static Item* createItem(FarmingObject::TypeID type) {
        switch (type) {
            case FarmingObject::TypeID::ITEM_PRODUCE_TOMATO: return new ItemProduceTomato();
            case FarmingObject::TypeID::ITEM_PRODUCE_CARROT: return new ItemProduceCarrot();
            case FarmingObject::TypeID::ITEM_PRODUCE_BLUEBERRY: return new ItemProduceBlueberry();
            case FarmingObject::TypeID::ITEM_SEED_TOMATO: return new ItemSeedTomato();
            case FarmingObject::TypeID::ITEM_SEED_CARROT: return new ItemSeedCarrot();
            case FarmingObject::TypeID::ITEM_SEED_BLUEBERRY: return new ItemSeedBlueberry();


            case FarmingObject::TypeID::ITEM_GOLD: return new ItemGold();
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
            items.push_back(item);
        }
        return item;
    }

    static Tile* createTile(FarmingObject::TypeID type, ivec2 tile) {
        switch (type) {
            case FarmingObject::TypeID::TILE_PLANT_TOMATO: return new TilePlantTomato(tile);
            case FarmingObject::TypeID::TILE_PLANT_CARROT: return new TilePlantCarrot(tile);
            case FarmingObject::TypeID::TILE_PLANT_BLUEBERRY: return new TilePlantBlueberry(tile);
            default: return nullptr;
        }
    }

    [[nodiscard]] Tile* getTile(int index) const {
        if (index >= 0 && index < TILES_HORIZ * TILES_VERT) {
            return tiles[index];
        }
        return nullptr;

    }

    [[nodiscard]] Tile* getTile(ivec2 tile) const {
        return getTile(tile.y * TILES_HORIZ + tile.x);
    }

    void setTile(ivec2 tile, Tile* data) {
        tiles[tile.y * TILES_HORIZ + tile.x] = data;
    }

    void clearTile(ivec2 tile) {
        Tile *t = new Tile(tile);
        updateTile(tile, t);
    }

    void updateTile(ivec2 tile, Tile* data) {
        if (tiles[tile.y * TILES_HORIZ + tile.x] != nullptr) {
            delete tiles[tile.y * TILES_HORIZ + tile.x];
            tiles[tile.y * TILES_HORIZ + tile.x] = nullptr;
        }
        tiles[tile.y * TILES_HORIZ + tile.x] = data;
    }

};


#endif //ENGINE_2_WORLD_H