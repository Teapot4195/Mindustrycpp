//
// Created by teapot on 08/12/23.
//

#include "PixmapTextureData.h"

#include <utility>

#include <arc/graphics/Pixmap.h>
#include <arc/util/Exception.h>

PixmapTextureData::PixmapTextureData(std::shared_ptr<Pixmap> pixm, bool useMipMaps, bool disposePixmap)
    : pixmap(std::move(pixm)), uMipMaps(useMipMaps), dPixmap(disposePixmap), java::NamedTree("PixmapTextureData") {}

bool PixmapTextureData::disposePixmap() {
    return dPixmap;
}

std::shared_ptr<Pixmap> PixmapTextureData::consumePixmap() {
    return pixmap;
}

int PixmapTextureData::getWidth() {
    return pixmap->width;
}

int PixmapTextureData::getHeight() {
    return pixmap->height;
}

std::shared_ptr<Format> PixmapTextureData::getFormat() {
    return Format::rgba8888;
}

bool PixmapTextureData::useMipMaps() {
    return uMipMaps;
}

bool PixmapTextureData::isCustom() {
    return false;
}

void PixmapTextureData::consumeCustomData(int &target) {
    throw ArcRuntimeException("This TextureData implementation does not upload data itself");
}

bool PixmapTextureData::isPrepared() {
    return true;
}

void PixmapTextureData::prepare() {
    throw ArcRuntimeException("preapre() must not be called on a PixmapTextureData instance as it is already prepared");
}
