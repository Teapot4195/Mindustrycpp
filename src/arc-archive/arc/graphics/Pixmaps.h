//
// Created by teapot on 28/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAPS_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAPS_H

#include <memory>

#include <arc/struct.h>

// Forward Decl to avoid circular reference
class Pixmap;

class Pixmaps {
private:
    constexpr static const int offsets[] = {1, 0, 1, 1, 0, 1, -1, 1, -1, 0, -1, -1, 0, -1, 1, -1};
    static std::shared_ptr<Pixmap> drawPixmap;
    static IntSeq tmpArray;

    static std::shared_ptr<Pixmap> noise(int w, int h);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAPS_H
