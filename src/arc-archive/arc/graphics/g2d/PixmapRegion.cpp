//
// Created by teapot on 28/11/23.
//

#include "PixmapRegion.h"

#include <arc/graphics/Pixmap.h>
#include <arc/graphics/Color.h>

PixmapRegion::PixmapRegion(std::shared_ptr<Pixmap> pixmap, std::size_t x, std::size_t y, std::size_t width,
                           std::size_t height) {
    set(pixmap, x, y, width, height);
}

PixmapRegion::PixmapRegion(std::shared_ptr<Pixmap> pixmap) {
    set(pixmap);
}

int PixmapRegion::get(int x1, int y1) {
    return pixmap->get(x + x1, y + y1);
}

int PixmapRegion::getRaw(int x1, int y1) {
    return pixmap->getRaw(x + x1, y + y1);
}

int PixmapRegion::getA(int x1, int y1) {
    return pixmap->getA(x + x1, y + y1);
}

int PixmapRegion::get(int x1, int y1, std::shared_ptr<Color> color) {
    int c = get(x1, y1);
    color->set(c);
    return c;
}

std::shared_ptr<PixmapRegion> PixmapRegion::set(std::shared_ptr<Pixmap> pixmap) {
    return set(pixmap, 0, 0, pixmap->width, pixmap->height);
}

std::shared_ptr<PixmapRegion>
PixmapRegion::set(std::shared_ptr<Pixmap> pixmap1, int x1, int y1, int width1, int height1) {
    pixmap = pixmap1;
    x = x1;
    y = y1;
    width = width1;
    height = height1;
    return shared_from_this();
}

std::shared_ptr<Pixmap> PixmapRegion::crop() {
    return Pixmaps::crop(pixmap, x, y, width, height);
}

std::shared_ptr<Pixmap> PixmapRegion::crop(int x1, int y1, int width1, int height1) {
    return Pixmaps::crop(pixmap, x + x1, y + y1, width1, height);
}
