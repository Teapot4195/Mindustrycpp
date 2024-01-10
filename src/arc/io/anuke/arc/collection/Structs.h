//
// Created by teapot on 08/01/24.
//

#ifndef MINDUSTRY_SRC_ARC_IO_ANUKE_ARC_COLLECTION_STRUCTS_H
#define MINDUSTRY_SRC_ARC_IO_ANUKE_ARC_COLLECTION_STRUCTS_H

#include <vector>

template <typename T>
bool inBounds(int x, int y, std::vector<std::vector<T>> array) {
    return x >= 0 && y >= 0 && x < array.size() && y < array[0].size();
}

template <typename T>
bool inBounds(int x, int y, int z, std::vector<std::vector<std::vector<T>>> array) {
    return x >= 0 && y >= 0 && z >= 0 && x < array.size() && y < array[0].size() && z < array[0][0].size();
}

bool inBounds(int x, int y, int z, int size, int padding);

template <typename T>
bool inBounds(int x, int y, int width, int height) {
    return x >= 0 && y >= 0 && x < width && y < height;
}

#endif //MINDUSTRY_SRC_ARC_IO_ANUKE_ARC_COLLECTION_STRUCTS_H
