//
// Created by teapot on 01/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_FILETEXTUREDATA_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_FILETEXTUREDATA_H

#include <arc/graphics/TextureData.h>

// Forward decl to avoid circular reference
class Fi;
class Pixmap;

class FileTextureData : public TextureData, public virtual java::NamedTree {
private:
    std::shared_ptr<Fi> file;
    int width = 0;
    int height = 0;
    std::shared_ptr<Pixmap> pixmap;
    bool useMipMaps_;
    bool isPrepared_ = false;

public:
    FileTextureData(std::shared_ptr<Fi> file, std::shared_ptr<Pixmap> preloadedPixmap, bool useMipMaps);

    bool isPrepared() override;

    void prepare() override;

    std::shared_ptr<Pixmap> consumePixmap() override;

    bool disposePixmap() override;

    int getWidth() override;

    int getHeight() override;

    std::shared_ptr<Format> getFormat() override;

    bool useMipMaps() override;

    std::shared_ptr<Fi> getFileHandle();

    bool isCustom() override;

    void consumeCustomData(int &target) override;

    std::string toString();
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_FILETEXTUREDATA_H
