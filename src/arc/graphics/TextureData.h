//
// Created by teapot on 01/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_TEXTUREDATA_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_TEXTUREDATA_H

#include <memory>

// Forward decl to avoid circular reference
class Pixmap;
class Fi;
enum class Format;

class TextureData {
public:
    virtual ~TextureData() = default;
    virtual bool isCustom() = 0;
    virtual bool isPrepared() = 0;
    virtual void preapre() = 0;
    virtual std::shared_ptr<Pixmap> consumePixmap() = 0;
    virtual std::shared_ptr<Pixmap> getPixmap();
    virtual bool disposePixmap() = 0;
    virtual void consumeCustomData(int &target) = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual std::shared_ptr<Format> getFormat() = 0;
    virtual bool useMipMaps() = 0;

    static std::shared_ptr<TextureData> load(std::shared_ptr<Fi> file, bool useMipMaps);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_TEXTUREDATA_H
