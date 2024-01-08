//
// Created by teapot on 28/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_G2D_PIXMAPREGION_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_G2D_PIXMAPREGION_H

#include <memory>

// Forward decl to avoid circular reference
class Pixmap;
class Color;

class PixmapRegion : public std::enable_shared_from_this<PixmapRegion> {
public:
    std::shared_ptr<Pixmap> pixmap;
    std::size_t x, y, width, height;

    PixmapRegion(std::shared_ptr<Pixmap> pixmap, std::size_t x, std::size_t y, std::size_t width, std::size_t height);

    PixmapRegion(std::shared_ptr<Pixmap> pixmap);

    int get(int x1, int y1);

    int getRaw(int x1, int y1);

    int getA(int x1, int y1);

    int get(int x1, int y1, std::shared_ptr<Color> color);

    std::shared_ptr<PixmapRegion> set(std::shared_ptr<Pixmap> pixmap);

    std::shared_ptr<PixmapRegion> set(std::shared_ptr<Pixmap> pixmap1, int x1, int y1, int width1, int height1);

    std::shared_ptr<Pixmap> crop();

    std::shared_ptr<Pixmap> crop(int x1, int y1, int width1, int height1);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_G2D_PIXMAPREGION_H
