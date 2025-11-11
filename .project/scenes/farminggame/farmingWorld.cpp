//
// Created by cobble on 11/3/2025.
//

#include "farmingWorld.h"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "farmingGame.h"
#include "FarmingObject.h"
#include "littleGuy.h"
#include "items/Item.h"
#include "items/produce/ItemProduceCarrot.h"
#include "items/seeds/ItemSeedCarrot.h"
#include "items/produce/ItemProduceTomato.h"
#include "items/seeds/ItemSeedTomato.h"
#include "tiles/Tile.h"
#include "tiles/plants/TilePlant.h"
#include "tiles/plants/TilePlantCarrot.h"
#include "tiles/plants/TilePlantTomato.h"

Tiles2d FarmingWorld::landTilemap = Tiles2d();
Tiles2d FarmingWorld::plantTilemap = Tiles2d();
Texture2d FarmingWorld::barnTexture = Texture2d();
MultiTexture2d FarmingWorld::uiTexture = MultiTexture2d();
FontRenderer *FarmingWorld::fontRenderer = nullptr;

FarmingWorld::FarmingWorld() {
    this->savePath = "";
}

FarmingWorld::FarmingWorld(const std::string &savePath) {
    this->savePath = savePath;
    LittleGuy::setWorld(this);
    FarmingObject::setWorld(this);
    load();
}

FarmingWorld::~FarmingWorld() {
    cleanup();
}

void FarmingWorld::load() {
    for (int i = 0; i < TILES_HORIZ * TILES_VERT; i++) {
        landData[i] = FARMLAND;
        plantData[i] = EMPTY;
        tiles.push_back(nullptr);
    }
    items.clear();
    inventory[FarmingObject::TypeID::ITEM_SEED_TOMATO] = 100;
    inventory[FarmingObject::TypeID::ITEM_SEED_CARROT] = 100;

    std::map<FarmingObject::TypeID, std::string> itemTypes;

    enum LoadState {
        LAND,
        TILES,
        ITEMS,
        LITTLE_GUYS,
        INVENTORY
    };

    LoadState state = LAND;

    if (!savePath.empty()) {
        std::ifstream saveFileData(savePath);
        if (saveFileData.is_open()) {
            FarmingObject *object = nullptr;
            std::string line;
            int i = 0;
            LittleGuy *currentGuy = nullptr;
            FarmingObject *currentObject = nullptr;
            while (std::getline(saveFileData, line)) {
                if (line.empty()) continue;
                LoadState prevState = state;
                //std::cout << "'" << line << "'" << std::endl;
                if (line == "LAND") state = LAND;
                else if (line == "TILES") state = TILES;
                else if (line == "ITEMS") state = ITEMS;
                else if (line == "LITTLE_GUYS") state = LITTLE_GUYS;
                else if (line == "INVENTORY") state = INVENTORY;
                if (prevState != state && object != nullptr) {
                    switch (prevState) {
                        case TILES: {
                            if (getTile(object->tile) == nullptr) {
                                setTile(object->tile, dynamic_cast<Tile*>(object));
                            }
                            object = nullptr;
                            break;
                        }

                        case ITEMS: items.push_back(dynamic_cast<Item*>(object)); break;
                        default: break;
                    }
                }
                for (auto const& [type, data] : FarmingObject::objectData) {
                    if (line == "="+data->configKey || line == "LITTLE_GUY_START") {
                        i = 0;

                        if (object != nullptr) {switch (state) {
                            case ITEMS: items.push_back(dynamic_cast<Item*>(object)); break;
                            case TILES: setTile(object->tile, dynamic_cast<Tile*>(object)); break;
                            default: break;
                        }
                        }

                    }
                }
                std::cout << "["  << i << "] '" << line << "'" << std::endl;
                switch (state) {
                    case LAND: {
                        if (i > TILES_VERT * TILES_HORIZ) continue;
                        std::istringstream iss(line);
                        int number;
                        while (iss >> number && i < TILES_HORIZ * TILES_VERT) {
                            landData[i] = number;
                            i++;
                        }
                        break;
                    }
                    case TILES: {
                        if (i == 0) {
                            bool found = false;
                            for (auto const& [type, data] : FarmingObject::objectData) {
                                if (dynamic_cast<Tile::TileData*>(data)) { //check if tile
                                    if ("="+data->configKey == line) {
                                        object = createTile(type, ivec2(0));
                                        found = true;
                                        break;
                                    }
                                }
                            }
                            if (!found) std::cout << "UNKNOWN TILE IS BEING LOADED!!" << std::endl << "Tile: '" << line << "'" << std::endl;
                        } else {
                            if (object != nullptr) object->loadConfig(line, i);
                        }
                        break;
                    }
                    case ITEMS: {

                        if (i == 0) {
                            bool found = false;
                            for (auto const& [type, data] : FarmingObject::objectData) {
                                if (dynamic_cast<Item::ItemData*>(data)) { //check if item
                                    if ("="+data->configKey == line) {
                                        object = createItem(type);
                                        found = true;
                                        break;
                                    }
                                }
                            }
                            if (!found) std::cout << "UNKNOWN ITEM IS BEING LOADED!!" << std::endl << "Item: '" << line << "'" << std::endl;
                        } else {

                            if (object != nullptr) object->loadConfig(line, i - 1);
                        }
                        break;
                    }
                    case LITTLE_GUYS: {
                        if (line == "LITTLE_GUY_START") object = new LittleGuy();
                        else if (line == "LITTLE_GUY_END") guys.push_back(dynamic_cast<LittleGuy *>(object));
                        else if (object != nullptr) object->loadConfig(line, i - 1);
                        break;
                    }
                    case INVENTORY: {
                        std::string itemTextBuilder;
                        std::string amountTextBuilder;
                        bool modeBuilder = true;
                        for (int j = 0; j < line.length(); j++) {
                            if (modeBuilder) {
                                if (line[j] == ':') {
                                    modeBuilder = false;
                                } else {
                                    itemTextBuilder += line[j];
                                }
                            } else {
                                amountTextBuilder += line[j];
                            }
                        }

                        for (const auto&[type, data] : FarmingObject::objectData) {
                            if (FarmingObject::getData<FarmingObject::ObjectData>(type)->configKey == itemTextBuilder) {
                                inventory[type] = std::stoi(amountTextBuilder);
                                break;
                            }
                        }

                        break;
                    }
                }
                i++;
            }
            saveFileData.close();
        } else {
            std::cout << "Failed to open save file at: " << savePath << std::endl;
        }

        updateTileTypes();
    }

    for (int j = 0; j < TILES_HORIZ * TILES_VERT; j++) {
        if (getTile(ivec2(j % TILES_HORIZ, j / TILES_HORIZ)) == nullptr) {
            setTile(ivec2(j % TILES_HORIZ, j / TILES_HORIZ), new Tile(ivec2(j % TILES_HORIZ, j / TILES_HORIZ)));
        }
    }

    std::cout << "Loaded world from file! (" << std::to_string(tiles.size()) + " tiles, " << std::to_string(items.size()) << " items, and " << guys.size() << " guys)" << std::endl;

    //if (guys.empty()) guys.emplace_back(new LittleGuy());
}

void FarmingWorld::save() const {
    if (!savePath.empty()) {
        std::filesystem::create_directories(std::filesystem::path(savePath).parent_path());
        std::ofstream saveFileData(savePath);
        if (saveFileData.is_open()) {
            std::string output = "LAND\n";
            for (int y = 0; y < TILES_VERT; y++) {
                for (int x = 0; x < TILES_HORIZ; x++) {
                    output += std::to_string(landData[y * TILES_HORIZ + x]);
                    if (x != TILES_HORIZ - 1) output += " ";
                }
                output += "\n";
            }
            output += "TILES\n";
            for (auto tile : tiles) {
                if (tile->exists()) {
                    output += "="+FarmingObject::getData<FarmingObject::ObjectData>(tile->getType())->configKey + "\n";
                    output += tile->getConfig();
                }
            }
            output += "ITEMS\n";
            for (auto item : items) {
                output += "="+FarmingObject::getData<FarmingObject::ObjectData>(item->getType())->configKey + "\n";
                output += item->getConfig();
            }
            output += "LITTLE_GUYS\n";
            for (auto guy : guys) {
                output += "LITTLE_GUY_START\n";
                output += guy->getConfig();
                output += "LITTLE_GUY_END\n";
            }
            output += "INVENTORY\n";
            for (auto const& [type, amount] : inventory) {
                output += FarmingObject::getData<FarmingObject::ObjectData>(type)->configKey + ":" + std::to_string(amount) + "\n";
            }

            saveFileData << output;
            saveFileData.close();
            std::cout << "Saved world!" << std::endl;
        } else {
            std::cout << "Failed to save file at: " << savePath << std::endl;
        }
    } else {
        std::cout << "No save file location set!" << std::endl;
    }
}

void FarmingWorld::cleanup() {
    for (auto & item : items) {
        if (item != nullptr) {
            delete item;
            item = nullptr;
        }
    }
    items.clear();
    for (auto & tile : tiles) {
        if (tile != nullptr) {
            delete tile;
            tile = nullptr;
        }
    }
    tiles.clear();
    for (auto & guy : guys) {
        if (guy != nullptr) {
            delete guy;
            guy = nullptr;
        }
    }
    guys.clear();
}

void FarmingWorld::update(float dt) {
    for (auto & guy : guys) {
        guy->update(dt);
    }
}

void FarmingWorld::tick() {
    //std::cout << "\n==========================================================================\n" << std::endl;
    for (auto & guy : guys) {
        //if (guy->getTask() != nullptr) std::cout << guy->getTask()->getName() << std::endl;
        guy->tick();

    }
    for (auto & tile : tiles) {
        if (tile != nullptr) {
            tile->tick();
        }
    }
}

bool FarmingWorld::isFarmland(int texIndex) {
    return texIndex == FARMLAND || (texIndex % 16 <= 11 && texIndex >= 16 && texIndex <= 79);
}

bool FarmingWorld::isFarmland(int x, int y) const {
    return x < 0 || y < 0 || x >= TILES_HORIZ || y >= TILES_VERT ? false : isFarmland(landData[y * TILES_HORIZ + x]);
}

void FarmingWorld::updateTileTypes() {
    for (int i = 0; i < TILES_VERT * TILES_HORIZ; i++) {
        int x = i % TILES_HORIZ;
        int y = i / TILES_HORIZ;
        if (isFarmland(landData[i])) {
            int subType;


            bool topMiddle = isFarmland(x, y + 1);
            bool middleLeft = isFarmland(x - 1, y);
            bool middleRight = isFarmland(x + 1, y);
            bool bottomMiddle = isFarmland(x, y - 1);

            bool topLeft = isFarmland(x - 1, y + 1) && (topMiddle && middleLeft);
            bool topRight = isFarmland(x + 1, y + 1) && (topMiddle && middleRight);
            bool bottomLeft = isFarmland(x - 1, y - 1) && (bottomMiddle && middleLeft);
            bool bottomRight = isFarmland(x + 1, y - 1) && (bottomMiddle && middleRight);


            uint8_t mask = (static_cast<uint8_t>(topLeft) << 0) |
               (static_cast<uint8_t>(topMiddle) << 1) |
               (static_cast<uint8_t>(topRight) << 2) |
               (static_cast<uint8_t>(middleLeft) << 3) |
               (static_cast<uint8_t>(middleRight) << 4) |
               (static_cast<uint8_t>(bottomLeft) << 5) |
               (static_cast<uint8_t>(bottomMiddle) << 6) |
               (static_cast<uint8_t>(bottomRight) << 7);

            //std::cout << "Bitmask: " << std::bitset<8>(mask) << "\n";

            switch (mask) {
                case 0b11010000: subType =  0; break;
                case 0b11111000: subType =  1; break;
                case 0b01101000: subType =  2; break;
                case 0b01000000: subType =  3; break;
                case 0b01010000: subType =  4; break;
                case 0b01111000: subType =  5; break;
                case 0b11011000: subType =  6; break;
                case 0b01001000: subType =  7; break;
                case 0b01011000: subType =  8; break;
                case 0b01010010: subType =  9; break;
                case 0b11011011: subType = 10; break;
                case 0b01011010: subType = 11; break;
                case 0b11010110: subType = 16; break;
                case 0b11111111: subType = 17; break;
                case 0b01101011: subType = 18; break;
                case 0b01000010: subType = 19; break;
                case 0b01010110: subType = 20; break;
                case 0b01111111: subType = 21; break;
                case 0b11011111: subType = 22; break;
                case 0b01001011: subType = 23; break;
                case 0b01011111: subType = 24; break;
                case 0b01111011: subType = 25; break;
                case 0b01111110: subType = 26; break;
                case 0b00010110: subType = 32; break;
                case 0b00011111: subType = 33; break;
                case 0b00001011: subType = 34; break;
                case 0b00000010: subType = 35; break;
                case 0b11010010: subType = 36; break;
                case 0b11111011: subType = 37; break;
                case 0b11111110: subType = 38; break;
                case 0b01101010: subType = 39; break;
                case 0b11111010: subType = 40; break;
                case 0b11011110: subType = 41; break;
                case 0b11011010: subType = 42; break;
                case 0b01111010: subType = 43; break;
                case 0b00010000: subType = 48; break;
                case 0b00011000: subType = 49; break;
                case 0b00001000: subType = 50; break;
                case 0b00000000: subType = 51; break;
                case 0b00010010: subType = 52; break;
                case 0b00011011: subType = 53; break;
                case 0b00011110: subType = 54; break;
                case 0b00001010: subType = 55; break;
                case 0b00011010: subType = 56; break;
                case 0b01001010: subType = 57; break;
                case 0b01011110: subType = 58; break;
                case 0b01011011: subType = 59; break;
                default: subType = 31; break; //error space (smiley face)
            }

            landData[i] = subType + FARMLAND_TEXTURE_OFFSET;
        }
    }
}

void FarmingWorld::clearObjects() {
    for (auto & tile : tiles) {
        if (dynamic_cast<TilePlant*>(tile)) {
            plantData[tile->tile.y * TILES_HORIZ + tile->tile.x] = 0;
        }
        ivec2 p = tile->tile;
        delete tile;
        tile = new Tile(ivec2(p.x, p.y));
    }
    for (auto & guy : guys) {
        if (guy != nullptr) {
            guy->clearObjects();
            delete guy;
            guy = nullptr;
        }
    }
    guys.clear();
    for (auto & item : items) {
        if (item != nullptr) {
            delete item;
            item = nullptr;
        }
    }
    items.clear();
}


vec2 FarmingWorld::getTilePos(int x, int y) {
    return {TILE_OFFSET_X + (x + 0.5f) * TILE_WIDTH, TILE_OFFSET_Y + (y + 0.5f) * TILE_HEIGHT};
}

void FarmingWorld::draw() const {
    Texture2d::setColor(vec4(1));
    landTilemap.draw(TILE_OFFSET_X, TILE_OFFSET_Y, TILE_WIDTH * TILES_HORIZ, TILE_HEIGHT * TILES_VERT, true);
    plantTilemap.draw(TILE_OFFSET_X, TILE_OFFSET_Y + TILE_HEIGHT * 0.2f, TILE_WIDTH * TILES_HORIZ, TILE_HEIGHT * TILES_VERT, true);

    Texture2d::setColor(vec4(1));
    for (auto guy : guys) {
        guy->draw(true);
    }

    for (int i = 0; i < items.size(); i++) {
        items[i]->draw(i == 0);
    }
    barnTexture.draw(TILE_OFFSET_X - 2 * TILE_WIDTH + TILE_WIDTH * (4.0f / 18.0f), TILE_OFFSET_Y, TILE_WIDTH * 2, TILE_HEIGHT * 6, true);


    int inventoryIndex = 0;
    for (const auto&[fst, snd] : inventory) {
        uiTexture.draw(inventoryIndex * TILE_WIDTH, Window::GAME_HEIGHT - TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, 0);
        Item::draw(inventoryIndex * TILE_WIDTH, Window::GAME_HEIGHT - TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, FarmingObject::getData<Item::ItemData>(fst)->textureIndex);
        fontRenderer->setColor(vec4(1));
        fontRenderer->draw(std::to_string(snd), inventoryIndex * TILE_WIDTH + (TILE_WIDTH - fontRenderer->getWidth(std::to_string(snd)) * 3) - (TILE_WIDTH / 18.0f), Window::GAME_HEIGHT - TILE_HEIGHT + (TILE_HEIGHT / 18.0f), 3);
        inventoryIndex++;
    }


}

ivec2 FarmingWorld::getTileFromPos(vec2 pos) {
    return (pos - vec2(TILE_OFFSET_X, TILE_OFFSET_Y)) / vec2(TILE_WIDTH, TILE_HEIGHT);
}

