//
// Created by teapot on 03/12/23.
//

#include "MipMapGenerator.h"

#include <utility>

#include <arc/Application.h>
#include <arc/Core.h>
#include <arc/Graphics.h>
#include <arc/graphics/Gl.h>
#include <arc/graphics/GL20.h>
#include <arc/graphics/Pixmap.h>
#include <arc/util/Exception.h>

bool MipMapGenerator::useHWMipMap = true;

void MipMapGenerator::setUserHardwareMipMap(bool useHWMipMap1) {
    MipMapGenerator::useHWMipMap = useHWMipMap1;
}

void MipMapGenerator::generateMipMap(std::shared_ptr<Pixmap>& pixmap, int textureWidth, int textureHeight) {
    generateMipMap(GL20::GL_TEXTURE_2D, pixmap, textureWidth, textureHeight);
}

void MipMapGenerator::generateMipMap(int target, std::shared_ptr<Pixmap>& pixmap, int textureWidth, int textureHeight) {
    if (!useHWMipMap) {
        generateMipMapCPU(target, pixmap, textureWidth, textureHeight);
        return;
    }

    if (Core::app.value()->isAndroid() || Core::app.value()->isWeb() || Core::app.value()->isIOS())
        generateMipMapGLES20(target, pixmap);
    else
        generateMipMapDesktop(target, pixmap, textureWidth, textureHeight);
}

void MipMapGenerator::generateMipMapGLES20(int target, std::shared_ptr<Pixmap> &pixmap) {
    Gl::texImage2D(target, 0, pixmap->getGlInternalFormat(), pixmap->width, pixmap->height, 0,
                   pixmap->getGLFormat(), pixmap->getGLType(), pixmap->pixels);
    Gl::generateMipmap(target);
}

void MipMapGenerator::generateMipMapDesktop(int target, std::shared_ptr<Pixmap> &pixmap, int textureWidth,
                                            int textureHeight) {
    if (Core::graphics.value()->supportsExtension("GL_ARB_framebuffer_object") ||
        Core::graphics.value()->supportsExtension("GL_EXT_framebuffer_object") ||
        Core::gl30.has_value()) {
        Gl::texImage2D(target, 0, pixmap->getGLInternalFormat(), pixmap->width, pixmap->height, 0,
                       pixmap->getGLFormat(), pixmap->getGLType(), pixmap->pixels);
        Gl::generateMipmap(target);
    } else
        generateMipMapCPU(target, pixmap, textureWidth, textureHeight);
}

void
MipMapGenerator::generateMipMapCPU(int target, std::shared_ptr<Pixmap> &pixmap, int textureWidth, int textureHeight) {
    Gl::texImage2D(target, 0, pixmap->getGLInternalFormat(), pixmap->width, pixmap->height, 0,
                   pixmap->getGLFormat(), pixmap->getGLType(), pixmap->pixels);
    if ((!Core::gl20.has_value()) && textureWidth != textureHeight)
        throw ArcRuntimeException("texture width and height must be square when using mipmapping.");
    int width = pixmap->width / 2;
    int height = pixmap->height / 2;
    int level = 1;
    while (width > 0 && height > 0) {
        std::shared_ptr<Pixmap> tmp = std::make_shared<Pixmap>(width, height);
        tmp->draw(pixmap, 0, 0, pixmap->width, pixmap->height, 0, 0, width, height);
        if (level > 1)
            pixmap->dispose();
        pixmap = tmp;

        Gl::texImage2D(target, level, pixmap->getGLInternalFormat(), pixmap->width, pixmap->height, 0,
                       pixmap->getGLFormat(), pixmap->getGLType(), pixmap->pixels);

        width = pixmap->width / 2;
        height = pixmap->height / 2;
        level++;
    }
}
