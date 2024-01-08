//
// Created by teapot on 04/12/23.
//

#include "Graphics.h"

#include <utility>

#include <arc/Core.h>
#include <arc/Files.h>
#include <arc/graphics/Gl.h>
#include <arc/graphics/GL20.h>
#include <arc/graphics/Pixmap.h>
#include <arc/graphics/Pixmaps.h>
#include <arc/math/Mathf.h>
#include <arc/util/OS.h>

BufferFormat::BufferFormat(int r, int g, int b, int a, int depth, int stencil, int samples, bool coverageSamppling)
  : r(r),
    g(g),
    b(b),
    a(a),
    depth(depth),
    stencil(stencil),
    samples(samples),
    coverageSampling(coverageSamppling) {}

std::string BufferFormat::toString() const {
    return "r: " + std::to_string(r) +
           ", g: " + std::to_string(g) +
           ", b: " + std::to_string(b) +
           ", a: " + std::to_string(a) +
           ", depth: " + std::to_string(depth) +
           ", stencil: " + std::to_string(stencil) +
           ", num samples: " + std::to_string(samples) +
           ", coverage sampling: " + (coverageSampling ? "true" : "false");
}

void SystemCursor::set(std::shared_ptr<Cursor> curs) {
    cursor = std::move(curs);
}

SystemCursor::SystemCursor() : NamedTree("") {}

void SystemCursor::dispose() {
    if (!cursor) {
        cursor->dispose();
        cursor.reset();
    }
}

Cursor::Cursor() : NamedTree("Cursor") {}

bool Graphics::isGL30Available() {
    return !Core::gl30;
}

void Graphics::clear(float r, float g, float b, float a) {
    Gl::clearColor(r, g, b, a);
    Gl::clear(GL20::GL_COLOR_BUFFER_BIT);
}

bool Graphics::isPortrait() {
    return getWidth() < getHeight();
}

float Graphics::getAspect() {
    return (float)getWidth() / (float)getHeight();
}

bool Graphics::isHidden() {
    return getWidth() < 2 || getHeight() < 2;
}

std::vector<int> Graphics::getSaveInsets() {
    return std::vector<int>(4);
}

bool Graphics::setFullscreen() {
    return false;
}

std::shared_ptr<Cursor>
Graphics::newCursor(const std::shared_ptr<Pixmap>& pixmap, int scaling, std::shared_ptr<Color> outlineColor,
                    int outlineThickness) {
    std::shared_ptr<Pixmap> out = pixmap->outline(std::move(outlineColor), outlineThickness);
    std::shared_ptr<Pixmap> out2 = Pixmaps::scale(out, scaling);

    if(!Mathf::isPowerOfTwo(out2->width)){
        std::shared_ptr<Pixmap> old = out2;
        out2 = Pixmaps::resize(out2, Mathf::nextPowerOfTwo(out2->width), Mathf::nextPowerOfTwo(out2->width));
        old->dispose();
    }

    out->dispose();
    pixmap->dispose();

    return newCursor(out2, out2->width / 2, out2->height / 2);
}

std::shared_ptr<Cursor> Graphics::newCursor(const std::string& filename, int scale) {
    if(scale == 1 || OS::isAndroid || OS::isIos)
        return newCursor(filename);
    std::shared_ptr<Pixmap> base = std::make_shared<Pixmap>(Core::files.value()->internal("cursors/" + filename + ".png"));
    std::shared_ptr<Pixmap> result = Pixmaps::scale(base, base->width * scale, base->height * scale);
    base->dispose();
    return newCursor(result, result->width /2, result->height /2);
}

std::shared_ptr<Cursor> Graphics::newCursor(const std::string& filename) {
    std::shared_ptr<Pixmap> p = std::make_shared<Pixmap>(Core::files.value()->internal("cursors/" + filename + ".png"));
    return newCursor(p, p->width /2, p->height /2);
}

std::shared_ptr<Cursor>
Graphics::newCursor(const std::string& filename, int scaling, std::shared_ptr<Color> outlineColor, int outlineScaling) {
    return newCursor(std::make_shared<Pixmap>(Core::files.value()->internal("cursors/" + filename + ".png")), scaling, std::move(outlineColor), outlineScaling);
}

void Graphics::restoreCursor() {
    cursor(SystemCursor::arrow);
}

void Graphics::cursor(const std::shared_ptr<Cursor>& cursor) {
    if (lastCursor == cursor)
        return;

    if (cursor->getInstof() == "SystemCursor")
        if (!std::dynamic_pointer_cast<SystemCursor>(cursor)->cursor)
            setCursor(std::dynamic_pointer_cast<SystemCursor>(cursor)->cursor);
        else
            setSystemCursor(std::dynamic_pointer_cast<SystemCursor>(cursor));
    else
        setCursor(cursor);

    lastCursor = cursor;
}

void Graphics::dispose() {}
