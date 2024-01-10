//
// Created by teapot on 08/01/24.
//

#include "Structs.h"

bool inBounds(int x, int y, int z, int size, int padding) {
    return x >= padding && y >= padding && z >= padding && x < size - padding && y < size - padding && z < size - padding;
}
