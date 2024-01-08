//
// Created by teapot on 08/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_PIXMAPTEXTUREDATA_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_PIXMAPTEXTUREDATA_H

#include <arc/graphics/TextureData.h>

class PixmapTextureData : public TextureData, public virtual java::NamedTree  {
public:
    const std::shared_ptr<Pixmap> pixmap;

    const bool uMipMaps;
    const bool dPixmap;

    PixmapTextureData(std::shared_ptr<Pixmap> pixm, bool useMipMaps, bool disposePixmap);

    bool disposePixmap() override;

    std::shared_ptr<Pixmap> consumePixmap() override;

    int getWidth() override;

    int getHeight() override;

    std::shared_ptr<Format> getFormat() override;

    bool useMipMaps() override;

    bool isCustom() override;

    void consumeCustomData(int &target) override;

    bool isPrepared() override;

    void prepare() override;
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_PIXMAPTEXTUREDATA_H
