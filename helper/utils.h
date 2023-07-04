#ifndef UTILS_H
#define UTILS_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <string.h>

#define POKEMON_INFO_COUNT 6

using namespace std;
using namespace sf;

std::string toLowercase(const std::string& str){
    string result;
    for (char c : str) {
        result += tolower(c);
    }
    return result;
};

bool isRegularFile(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0) {
        return false;
    }
    
    return S_ISREG(fileInfo.st_mode);
}

int countPokemon(const std::string& folderPath) {
    int count = 0;
    DIR* directory = opendir(folderPath.c_str());
    if (directory == nullptr) {
        std::cout << "Failed to open directory: " << folderPath << std::endl;
        return 0;
    }
    
    dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
        std::string fileName(entry->d_name);
        std::string fullPath = folderPath + "/" + fileName;
        
        if (isRegularFile(fullPath) && fileName.size() >= 4 && fileName.substr(fileName.size() - 4) == ".txt") {
            ++count;
        }
    }
    
    closedir(directory);
    return count;
}

void scanFilesInFolder(const string& folderPath, string**& pokemonList) {
    DIR* directory = opendir(folderPath.c_str());
    if (directory == nullptr) {
        cout << "Failed to open directory: " << folderPath << endl;
        return;
    }

    dirent* entry;
    int pokemonCounter = 0;

    while ((entry = readdir(directory)) != nullptr) {
        string filePath = folderPath + "/" + entry->d_name;
        struct stat fileStat;
        if (stat(filePath.c_str(), &fileStat) == -1) {
            cout << "Failed to get file stat: " << filePath << endl;
            continue;
        }

        if (S_ISREG(fileStat.st_mode)) {
            if (strstr(entry->d_name, ".txt") != nullptr) {
                ifstream file(filePath);
                if (file.is_open()) {
                    string line;
                    int pokemonInfoCounter = 0;
                    while (getline(file, line)) {
                        if (pokemonInfoCounter < POKEMON_INFO_COUNT) {
                            pokemonList[pokemonCounter][pokemonInfoCounter] = line;
                            ++pokemonInfoCounter;
                        }
                    }
                    ++pokemonCounter;
                    file.close();
                } else {
                    cout << "Failed to open file: " << filePath << endl;
                }
            }
        }
    }
    closedir(directory);
}

Color getColor(string type){
    if(type == "fire"){
        return Color(255, 49, 49);
    } else if(type == "normal"){
        return Color(191, 180, 180);
    } else if(type == "grass"){
        return Color(99, 207, 62);
    } else if(type == "water"){
        return Color(82, 113, 255);
    } else if(type == "ground"){
        return Color(207, 98, 70);
    } else if(type == "rock"){
        return Color(182, 78, 40);
    } else if(type == "bug"){
        return Color(133, 191, 112);
    } else if(type == "electric"){
        return Color(250, 217, 39);
    } else if(type == "ice"){
        return Color(126, 209, 255);
    } else if(type == "fighting"){
        return Color(168, 10, 10);
    } else if(type == "poison"){
        return Color(223, 99, 196);
    } else if(type == "flying"){
        return Color(34, 189, 255);
    } else if(type == "psychic"){
        return Color(100, 45, 95);
    } else if(type == "dark"){
        return Color(41, 39, 39);
    } else if(type == "dragon"){
        return Color(120, 0, 255);
    } else if(type == "ghost"){
        return Color(129, 94, 168);
    } else if(type == "steel"){
        return Color(107, 107, 107);
    } else if(type == "fairy"){
        return Color(255, 2, 161);
    } else{
        return Color::White;
    }
}

string createSubstring(const std::string& str, char character) {
    size_t index = str.find(character);
    if (index != std::string::npos) {
        return str.substr(0, index);
    }
    return str;
}

string capitalizeFirstLetter(const std::string& str) {
    if (str.empty()) {
        return str;
    }

    std::string result = str;
    result[0] = std::toupper(result[0]);
    return result;
}

bool isLastCharacterEqualTo(const std::string& str, char ch) {
    if (!str.empty()) {
        return str.back() == ch;
    }
    return false;
}

bool isSubstringPresent(const std::string& str, const std::string& substring) {
    return str.find(substring) != std::string::npos;
}

#endif
