//
// Created by teapot on 01/12/23.
//

#include "FileTextureData.h"

#include <arc/files/Fi.h>
#include <arc/graphics/Pixmap.h>
#include <util/Exception.h>

#include <utility>

FileTextureData::FileTextureData(std::shared_ptr<Fi> file, std::shared_ptr<Pixmap> preloadedPixmap, bool useMipMaps)
  : file(std::move(file)),
    pixmap(std::move(preloadedPixmap)),
    useMipMaps_(useMipMaps)
{
    if (!pixmap) {
        width = pixmap->width;
        height = pixmap->height;
    }
}

bool FileTextureData::isPrepared() {
    return isPrepared_;
}

void FileTextureData::preapre() {
    if (isPrepared_)
        throw ArcRuntimeException("Already Prepared");
    if (pixmap) {
        pixmap = std::make_shared<Pixmap>(file);
        width = pixmap->width;
        height = pixmap->height;
    }
    isPrepared_ = true;
}

std::shared_ptr<Pixmap> FileTextureData::consumePixmap() {
    if (!isPrepared_)
        throw ArcRuntimeException("Call prepare() before calling getPixmap()");
    isPrepared_ = false;
    auto p = pixmap;
    pixmap.reset();
    return p;
}

bool FileTextureData::disposePixmap() {
    return true;
}

int FileTextureData::getWidth() {
    return width;
}

int FileTextureData::getHeight() {
    return height;
}

std::shared_ptr<Format> FileTextureData::getFormat() {
    return Pixmap::Format::rgba8888;
}

bool FileTextureData::useMipMaps() {
    return useMipMaps_;
}

std::shared_ptr<Fi> FileTextureData::getFileHandle() {
    return file;
}

bool FileTextureData::isCustom() {
    return false;
}

void FileTextureData::consumeCustomData(int &target) {
    throw ArcRuntimeException("This TextureDAta implementation does not upload data itself");
}

std::string FileTextureData::toString() {
    return file->toString();
}
