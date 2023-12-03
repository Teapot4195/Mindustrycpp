//
// Created by teapot on 01/12/23.
//

#include "TextureData.h"

#include <arc/files/Fi.h>
#include <arc/graphics/PixmapIO.h>
#include <arc/graphics/gl/FileTextureData.h>

std::shared_ptr<TextureData> TextureData::load(std::shared_ptr<Fi> file, bool useMipMaps) {
    return std::make_shared<FileTextureData>(
            file,
            file->extEquals("apix") ? PixmapIO::readApix(file) : std::make_shared<Pixmap>(file),
            useMipMaps
    );
}

std::shared_ptr<Pixmap> TextureData::getPixmap() {
    if (!isPrepared())
        preapre();
    return consumePixmap();
}
