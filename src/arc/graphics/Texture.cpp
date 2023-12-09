//
// Created by teapot on 28/11/23.
//

#include "Texture.h"

#include <arc/Core.h>
#include <arc/Files.h>
#include <arc/files/Fi.h>
#include <arc/graphics/Gl.h>
#include <arc/graphics/Pixmap.h>
#include <arc/graphics/TextureData.h>
#include <arc/graphics/gl/FileTextureData.h>
#include <arc/graphics/gl/PixmapTextureData.h>

#include <utility>

std::vector<std::shared_ptr<TextureFilter>> TextureFilter::all {
    std::make_shared<TextureFilter>(TextureFilter::nearest),
    std::make_shared<TextureFilter>(TextureFilter::linear),
    std::make_shared<TextureFilter>(TextureFilter::mipMap),
    std::make_shared<TextureFilter>(TextureFilter::mipMapNearestNearest),
    std::make_shared<TextureFilter>(TextureFilter::mipMapLinearNearest),
    std::make_shared<TextureFilter>(TextureFilter::mipMapNearestLinear),
    std::make_shared<TextureFilter>(TextureFilter::mipMapLinearLinear)
};

TextureFilter::TextureFilter(TextureFilter::val e) : glEnum(e) {}

bool TextureFilter::isMipMap() const {
    return glEnum != GL20::GL_NEAREST && glEnum != GL20::GL_LINEAR;
}

std::vector<std::shared_ptr<TextureWrap>> TextureWrap::all {
        std::make_shared<TextureWrap>(TextureWrap::mirroredRepeat),
        std::make_shared<TextureWrap>(TextureWrap::clampToEdge),
        std::make_shared<TextureWrap>(TextureWrap::repeat)
};

TextureWrap::TextureWrap(int e) : glEnum(e) {}

int TextureWrap::getGLEnum() const {
    return glEnum;
}

Texture::Texture(std::string internalPath) : Texture(Core::files.value()->internal(std::move(internalPath))) {}

Texture::Texture(std::shared_ptr<Fi> file) : Texture(std::move(file), false) {}

Texture::Texture(std::shared_ptr<Fi> file, bool useMipMaps) : Texture(TextureData::load(std::move(file), useMipMaps)) {}

Texture::Texture(const std::shared_ptr<Pixmap>& pixmap) : Texture(std::make_shared<PixmapTextureData>(pixmap, false, false)) {}

Texture::Texture(const std::shared_ptr<Pixmap>& pixmap, bool useMipMaps) : Texture(std::make_shared<PixmapTextureData>(pixmap, useMipMaps, false)) {}

Texture::Texture(std::uint32_t width, std::uint32_t height) :
    Texture(std::make_shared<PixmapTextureData>(std::make_shared<Pixmap>(width, height, false, true))) {}

Texture::Texture(const std::shared_ptr<TextureData> &data) : Texture(GL20::GL_TEXTURE_2D, Gl::genTexture(), data) {}

Texture::Texture() : GLTexture(0, 0) {}

Texture::Texture(int glTarget, int glHandle, const std::shared_ptr<TextureData>& data) : GLTexture(glTarget, glHandle) {
    load(data);
}

std::shared_ptr<Texture> Texture::createEmpty(std::shared_ptr<TextureData> data) {
    std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture());
    tex->data = std::move(data);
    return tex;
}

void Texture::load(const std::shared_ptr<TextureData>& dat) {
    data = dat;
    width = dat->getWidth();
    height = dat->getHeight();

    if (!dat->isPrepared())
        dat->prepare();

    bind();
    uploadImageData((int)Gl::G::texture2d, data);

    unsafeSetFilter(minFilter, magFilter, true);
    unsafeSetWrap(uWrap, vWrap, true);
    Gl::bindTexture(glTarget, 0);
}

void Texture::draw(const std::shared_ptr<Pixmap>& pixmap) {
    draw(pixmap, 0, 0);
}

void Texture::draw(const std::shared_ptr<Pixmap>& pixmap, int x, int y) {
    bind();
    Gl::texSubImage2D(glTarget, 0, x, y, pixmap->width, pixmap->height, pixmap->getGLFormat(), pixmap->getGLType(), pixmap->pixels);
}

int Texture::getDpeth() {
    return 0;
}

std::shared_ptr<TextureData> Texture::getTextureData() const {
    return data;
}

bool Texture::isDisposed() {
    return glHandle == 0;
}

std::string Texture::toString() {
    if (data->getInstof() == "FileTextureData")
        return std::dynamic_pointer_cast<FileTextureData>(data)->toString();
    return "GLTexture at (" + util::int_to_hex(this) + ")";
}
