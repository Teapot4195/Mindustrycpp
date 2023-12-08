//
// Created by teapot on 03/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_MIPMAPGENERATOR_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_MIPMAPGENERATOR_H

#include <memory>

// Forward Decl to avoid circular reference
class Pixmap;

class MipMapGenerator {
private:
    static bool useHWMipMap;

public:
    MipMapGenerator() = delete;

    static void setUserHardwareMipMap(bool useHWMipMap1);

    static void generateMipMap(std::shared_ptr<Pixmap>& pixmap, int textureWidth, int textureHeight);

    static void generateMipMap(int target, std::shared_ptr<Pixmap>& pixmap, int textureWidth, int textureHeight);

    static void generateMipMapGLES20(int target, std::shared_ptr<Pixmap> &pixmap);

    static void generateMipMapDesktop(int target, std::shared_ptr<Pixmap> &pixmap, int textureWidth, int textureHeight);

    static void generateMipMapCPU(int target, std::shared_ptr<Pixmap> &pixmap, int textureWidth, int textureHeight);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_MIPMAPGENERATOR_H
