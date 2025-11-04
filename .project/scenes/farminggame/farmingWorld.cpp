//
// Created by cobble on 11/3/2025.
//

#include "farmingWorld.h"

#include <filesystem>
#include <iostream>


FarmingWorld::FarmingWorld() {
    this->tilesHoriz = 1;
    this->tilesVert = 1;
    this->savePath = "";
}

FarmingWorld::FarmingWorld(int tilesHoriz, int tilesVert, const std::string &savePath) {
    this->tilesHoriz = tilesHoriz;
    this->tilesVert = tilesVert;
    this->savePath = savePath;
    load();
}

FarmingWorld::~FarmingWorld() {
    cleanup();
}

void FarmingWorld::load() {
    for (int i = 0; i < tilesHoriz * tilesVert; i++) {
        landData[i] = static_cast<unsigned int>(3);
        structureData[i] = static_cast<unsigned int>(0);
    }


    enum LoadState {
        LAND,
        STRUCTURE
    };
    LoadState state = LAND;
    if (!savePath.empty()) {
        std::ifstream saveFileData(savePath);
        if (saveFileData.is_open()) {
            std::string line;
            int i = 0;
            while (std::getline(saveFileData, line) && i < tilesHoriz * tilesVert) {
                LoadState prevState = state;
                if (line == "LAND") state = LAND;
                else if (line == "STRUCTURE") state = STRUCTURE;

                if (prevState != state) {
                    i = 0;
                    continue;
                }

                switch (state) {
                    case LAND: {
                        std::istringstream iss(line);
                        int number;
                        while (iss >> number && i < tilesHoriz * tilesVert) {
                            landData[i] = number;
                            i++;
                        }
                        break;
                    }
                    case STRUCTURE: {
                        std::istringstream iss(line);
                        int number;
                        while (iss >> number && i < tilesHoriz * tilesVert) {
                            structureData[i] = number;
                            i++;
                        }
                        break;
                    }
                }

            }
            saveFileData.close();
        } else {
            std::cout << "Failed to open save file at: " << savePath << std::endl;
        }



    }
    std::cout << "Loaded world!" << std::endl;
}

void FarmingWorld::save() const {
    if (!savePath.empty()) {
        std::filesystem::create_directories(std::filesystem::path(savePath).parent_path());
        std::ofstream saveFileData(savePath);
        if (saveFileData.is_open()) {
            std::string output = "LAND\n";
            for (int y = 0; y < tilesVert; y++) {
                for (int x = 0; x < tilesHoriz; x++) {
                    output += std::to_string(landData[y * tilesHoriz + x]);
                    if (x != tilesHoriz - 1) output += " ";
                }
                output += "\n";
            }
            output += "STRUCTURE\n";
            for (int y = 0; y < tilesVert; y++) {
                for (int x = 0; x < tilesHoriz; x++) {
                    output += std::to_string(structureData[y * tilesHoriz + x]);
                    if (x != tilesHoriz - 1) output += " ";
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

unsigned int FarmingWorld::getLandData(int index) const {
    if (index >= 0 && index < tilesHoriz * tilesVert) {
        return landData[index];
    } else {
        std::cout << "PANIC 1" << std::endl;
        return 0;
    }
}

void FarmingWorld::setLandData(int index, unsigned int value) {
    if (index >= 0 && index < tilesHoriz * tilesVert) {
        landData[index] = value;
    } else {
        std::cout << "PANIC 2" << std::endl;
    }

}

