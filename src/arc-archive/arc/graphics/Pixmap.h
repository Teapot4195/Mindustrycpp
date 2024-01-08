//
// Created by teapot on 22/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAP_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAP_H

#include <vector>
#include <cstdint>
#include <string>
#include <memory>

#include <util/Disposable.h>
#include <func.h>

// Forward decl to resolve circular reference
class Fi;
class Color;

class Pixmap : public Disposable {
public:
    int width, height;
    std::vector<std::uint8_t> pixels;

public:
    long long handle;

public:
    Pixmap(int width, int height);

    Pixmap(const std::vector<std::uint8_t>& encodeData);

    [[deprecated("WARN! Ensure this code is using the correct function!")]]
    Pixmap(const std::vector<std::uint8_t>& encodeData, int offset, int len);

    Pixmap(std::string file);

    Pixmap(const std::shared_ptr<Fi>& file);

    Pixmap(std::vector<std::uint8_t> buffer, int width, int height, bool _);

    std::shared_ptr<Pixmap> copy();

    void each(const Intc2& cons) const;

    void replace(const IntIntf& func);

    void fill(int color);

    void fill(std::shared_ptr<Color> color);

    bool in(int x, int y);

    std::shared_ptr<Pixmap> crop(int x, int y, int width, int height);

    std::shared_ptr<Pixmap> flipY();

    std::shared_ptr<Pixmap> flipX();

    std::shared_ptr<Pixmap> outline(std::shared_ptr<Color> color, int radius);

    std::shared_ptr<Pixmap> outline(int color, int radius);

    void drawLine(std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2, int color);

    void drwRect(std::size_t x, std::size_t y, std::size_t width1, std::size_t height1, int color);

    void circlePoints(std::size_t cx, std::size_t cy, std::size_t x, std::size_t y, int col);

    void drawCircle(std::size_t x, std::size_t y, std::size_t radius, int col);

    void draw(std::shared_ptr<PixmapRegion> region);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAP_H
