//
// Created by cobble on 11/3/2025.
//

#include "farmingWorld.h"

#include <filesystem>
#include <iostream>
#include "littleGuy.h"

Tiles2d FarmingWorld::landTilemap = Tiles2d();
Tiles2d FarmingWorld::plantTilemap = Tiles2d();

FarmingWorld::FarmingWorld() {
    this->savePath = "";
}

FarmingWorld::FarmingWorld(const std::string &savePath) {
    this->savePath = savePath;
    load();

}

FarmingWorld::~FarmingWorld() {
    cleanup();
}

void FarmingWorld::load() {
    for (int i = 0; i < TILES_HORIZ * TILES_VERT; i++) {
        landData[i] = EMPTY;
        plantData[i] = EMPTY;
    }

    enum LoadState {
        LAND,
        PLANT,
        PLANT_DURATION
    };
    LoadState state = LAND;
    if (!savePath.empty()) {
        std::ifstream saveFileData(savePath);
        if (saveFileData.is_open()) {
            std::string line;
            int i = 0;
            while (std::getline(saveFileData, line)) {
                LoadState prevState = state;
                //std::cout << "'" << line << "'" << std::endl;
                if (line == "LAND") state = LAND;
                else if (line == "PLANT") state = PLANT;
                else if (line == "PLANT_DURATION") state = PLANT_DURATION;

                if (prevState != state) {
                    i = 0;
                    continue;
                }

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
                    case PLANT: {
                        if (i > TILES_VERT * TILES_HORIZ) continue;
                        std::istringstream iss(line);
                        int number;
                        while (iss >> number && i < TILES_HORIZ * TILES_VERT) {
                            plantData[i] = number;
                            i++;
                        }
                        break;
                    }
                    case PLANT_DURATION: {

                        break;
                    }
                }

            }
            saveFileData.close();
        } else {
            std::cout << "Failed to open save file at: " << savePath << std::endl;
        }
        for (int i = 0; i < TILES_HORIZ * TILES_VERT; i++) {
            tiles[i].landType = landData[i];
            tiles[i].plantType = plantData[i];

            int landX = landData[i] % 16;
            int landY = landData[i] / 16;
            if (landY >= 1 && landY <= 4 && landX >= 0 && landX <= 11) {
                tiles[i].landType = FARMLAND;
            }

        }

        updateTileTypes();
    }
    std::cout << "Loaded world from file!" << std::endl;

    guys.emplace_back();


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
            output += "PLANT\n";
            for (int y = 0; y < TILES_VERT; y++) {
                for (int x = 0; x < TILES_HORIZ; x++) {
                    output += std::to_string(plantData[y * TILES_HORIZ + x]);
                    if (x != TILES_HORIZ - 1) output += " ";
                }
                output += "\n";
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

}

void FarmingWorld::update(float dt) {
    for (int i = 0; i < guys.size(); i++) {
        guys[i].update(dt);
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

FarmingWorld::Tile FarmingWorld::getTile(int x, int y) const {
    int index = y * TILES_HORIZ + x;
    if (index >= 0 && index < TILES_VERT * TILES_HORIZ) {
        return tiles[index];
    }
    return {};
}


unsigned int FarmingWorld::getLandData(int index) const {
    if (index >= 0 && index < TILES_VERT * TILES_HORIZ) {
        unsigned int data = landData[index];
        if (isFarmland(data)) return FARMLAND;
        return landData[index];
    }
    std::cout << "GET LAND OUT OF BOUNDS" << std::endl;
    return EMPTY;
}

void FarmingWorld::setLandData(int index, unsigned int value) {
    if (index >= 0 && index < TILES_VERT * TILES_HORIZ) {
        tiles[index].landType = index;
        landData[index] = value;
        updateTileTypes();
    } else {
        std::cout << "SET LAND OUT OF BOUNDS" << std::endl;
    }

}

unsigned int FarmingWorld::getStructureData(int index) const {
    return 0;
}

void FarmingWorld::setStructureData(int index, unsigned int value) {

}

vec2 FarmingWorld::getTilePos(int x, int y) {
    return {TILE_OFFSET_X + x * TILE_WIDTH, TILE_OFFSET_Y + y * TILE_HEIGHT};
}

void FarmingWorld::draw() const {
    Texture2d::setColor(vec4(1));
    landTilemap.draw(TILE_OFFSET_X, TILE_OFFSET_Y, TILE_WIDTH * TILES_HORIZ, TILE_HEIGHT * TILES_VERT, true);
    plantTilemap.draw(TILE_OFFSET_X, TILE_OFFSET_Y, TILE_WIDTH * TILES_HORIZ, TILE_HEIGHT * TILES_VERT, true);

    Texture2d::setColor(vec4(1));
    for (int i = 0; i < guys.size(); i++) {
        guys[i].draw(i == 0);
    }
}

ivec2 FarmingWorld::getTileFromPos(vec2 pos) {
    return (pos - vec2(TILE_OFFSET_X, TILE_OFFSET_Y)) / vec2(TILE_WIDTH, TILE_HEIGHT);
}

