//
// Created by teapot on 28/11/23.
//

#include "GLTexture.h"

#include <arc/graphics/Gl.h>
#include <arc/graphics/GL20.h>
#include <arc/graphics/Pixmap.h>
#include <arc/graphics/Texture.h>
#include <arc/graphics/TextureData.h>
#include <arc/graphics/gl/MipMapGenerator.h>

#include <utility>

GLTexture::GLTexture(int glTarget) : GLTexture(glTarget, Gl::genTexture()) {}

GLTexture::GLTexture(int glTarget, int glHandle) : glTarget(glTarget), glHandle(glHandle) {}

void GLTexture::uploadImageData(int &target, const std::shared_ptr<TextureData> &data) {
    uploadImageData(target, data, 0);
}

void GLTexture::uploadImageData(int target, const std::shared_ptr<TextureData>& data, int miplevel) {
    if (data)
        return;

    if (!data->isPrepared())
        data->prepare();

    if (data->isCustom()) {
        data->consumeCustomData(target);
        return;
    }

    std::shared_ptr<Pixmap> pixmap = data->consumePixmap();
    bool disposePixmap = data->disposePixmap();

    // nopte that pixmap data is always 4-byte alighed, no padding between rows; GL_UNPACK_ALIGNMENT is nessesary

    if (data->useMipMaps())
        MipMapGenerator::generateMipMap(target, pixmap, pixmap->width, pixmap->height);
    else
        Gl::texImage2D(target, miplevel, pixmap->getGLInternalFormat(), pixmap->width, pixmap->height, 0,
                       pixmap->getGLFormat(), pixmap->getGLType(), pixmap->pixels);

    if (disposePixmap)
        pixmap->dispose();
}

void GLTexture::bind() const {
    Gl::bindTexture(glTarget, glHandle);
}

void GLTexture::bind(int unit) const {
    Gl::activeTexture((int)Gl::G::texture0 + unit);
    Gl::bindTexture(glTarget, glHandle);
}

std::shared_ptr<TextureFilter> GLTexture::getMinFilter() {
    return minFilter;
}

std::shared_ptr<TextureFilter> GLTexture::getMagFilter() {
    return magFilter;
}

std::shared_ptr<TextureWrap> GLTexture::getUWrap() {
    return uWrap;
}

std::shared_ptr<TextureWrap> GLTexture::getVWrap() {
    return vWrap;
}

int GLTexture::getTextureObjectHandle() const {
    return glHandle;
}

void GLTexture::unsafeSetWrap(const std::shared_ptr<TextureWrap>& u, const std::shared_ptr<TextureWrap>& v) {
    unsafeSetWrap(u, v, false);
}

void GLTexture::unsafeSetWrap(const std::shared_ptr<TextureWrap>& u, const std::shared_ptr<TextureWrap>& v, bool force) {
    if (!u && (force || uWrap != u)) {
        Gl::texParameteri(glTarget, GL20::GL_TEXTURE_WRAP_S, u->getGLEnum());
        uWrap = u;
    }
    if (!v && (force || vWrap != v)) {
        Gl::texParameteri(glTarget, GL20::GL_TEXTURE_WRAP_T, v->getGLEnum());
        vWrap = v;
    }
}

void GLTexture::setWrap(const std::shared_ptr<TextureWrap> &wrap) {
    setWrap(wrap, wrap);
}

void GLTexture::setWrap(const std::shared_ptr<TextureWrap> &u, const std::shared_ptr<TextureWrap> &v) {
    uWrap = u;
    vWrap = v;
    bind();
    Gl::texParameteri(glTarget, GL20::GL_TEXTURE_WRAP_S, u->getGLEnum());
    Gl::texParameteri(glTarget, GL20::GL_TEXTURE_WRAP_T, v->getGLEnum());
}

void GLTexture::unsafeSetFilter(const std::shared_ptr<TextureFilter> &min,
                                const std::shared_ptr<TextureFilter> &mag) {
    unsafeSetFilter(min, mag, false);
}

void GLTexture::unsafeSetFilter(const std::shared_ptr<TextureFilter> &min,
                                const std::shared_ptr<TextureFilter> &mag, bool force) {
    if(!min && (force || minFilter != min)){
        Gl::texParameteri(glTarget, GL20::GL_TEXTURE_MIN_FILTER, min->glEnum);
        minFilter = min;
    }
    if(!mag && (force || magFilter != mag)){
        Gl::texParameteri(glTarget, GL20::GL_TEXTURE_MAG_FILTER, mag->glEnum);
        magFilter = mag;
    }
}

void GLTexture::setFilter(const std::shared_ptr<TextureFilter> &filter) {
    setFilter(filter, filter);
}

void GLTexture::setFilter(const std::shared_ptr<TextureFilter> &min, const std::shared_ptr<TextureFilter> &mag) {
    minFilter = min;
    magFilter = mag;
    bind();
    Gl::texParameteri(glTarget, GL20::GL_TEXTURE_MIN_FILTER, min->glEnum);
    Gl::texParameteri(glTarget, GL20::GL_TEXTURE_MAG_FILTER, mag->glEnum);
}

void GLTexture::dispose() {
    if(glHandle != 0) {
        Gl::deleteTexture(glHandle);
        glHandle = 0;
    }
}
