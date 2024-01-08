//
// Created by teapot on 22/11/23.
//

#include "Pixmap.h"

#include <utility>

#include <Core.h>
#include "Color.h"

Pixmap::Pixmap(int width, int height) {
    load(width, height);
}

Pixmap::Pixmap(const std::vector<std::uint8_t>& encodeData) : Pixmap(encodeData, 0, encodeData.size()) {}

Pixmap::Pixmap(const std::vector<std::uint8_t>& encodeData, int offset, int len) {
    load(encodeData, offset, len);
}

Pixmap::Pixmap(std::string file) : Pixmap(Core::files.has_value() ? Fi::get(file) : Core::files->internal(file)) {}

Pixmap::Pixmap(const std::shared_ptr<Fi>& file) {
    std::vector<std::uint8_t> bytes = file->readBytes();
    load(bytes, 0, bytes.size(), file->toString());
}

Pixmap::Pixmap(std::vector<std::uint8_t> buffer, int width, int height, bool _) :
    width(width),
    height(height),
    pixels(std::move(buffer)),
    handle(-1)
{}

std::shared_ptr<Pixmap> Pixmap::copy() {
    std::shared_ptr<Pixmap> out = std::make_shared<Pixmap>(width, height);
    std::copy(pixels.begin(), pixels.end(), out->pixels.begin());
    return out;
}

void Pixmap::each(const Intc2& cons) const {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            cons(x, y);
        }
    }
}

void Pixmap::replace(const IntIntf &func) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            setRaw(x, y, func(getRaw(x, y)));
        }
    }
}

void Pixmap::fill(int color) {
    int len = width * height * 4;
    for(int i = 0; i < len; i += 4) {
        auto loc = (int*)(pixels.data() + i);
        *loc = color;
    }
}

void Pixmap::fill(std::shared_ptr<Color> color) {
    return fill(color->rgba());
}

bool Pixmap::in(int x, int y) {
    return x >= 0 && y >= 0 && x < width && y < height;
}

std::shared_ptr<Pixmap> Pixmap::crop(int x, int y, int width, int height) {
    if (isDisposed())
        throw std::invalid_argument("input is disposed.");
    std::shared_ptr<Pixmap> pixmap = std::make_shared<Pixmap>(width, height);
    pixmap->draw(this, 0, 0, x, y, width, height);
    return pixmap;
}

std::shared_ptr<Pixmap> Pixmap::flipY() {
    std::shared_ptr<Pixmap> copy = std::make_shared<Pixmap>(width, height);

    for (std::size_t x = 0; x < width; x++) {
        for (std::size_t y = 0; y < height; y++) {
            copy->setRaw(x, height - 1 - y, getRaw(x, y));
        }
    }

    return copy;
}

std::shared_ptr<Pixmap> Pixmap::flipX() {
    std::shared_ptr<Pixmap> copy = std::make_shared<Pixmap>(width, height);

    for (std::size_t x = 0; x < width; x++) {
        for (std::size_t y = 0; y < height; y++) {
            copy->set(width - 1 - x, y, getRaw(x, y));
        }
    }

    return copy;
}

std::shared_ptr<Pixmap> Pixmap::outline(std::shared_ptr<Color> color, int radius) {
    return outline(color->rgba(), radius);
}

std::shared_ptr<Pixmap> Pixmap::outline(int color, int radius) {
    std::shared_ptr<Pixmap> pixmap = copy();

    for (std::size_t x = 0; x < width; x++) {
        for (std::size_t y = 0; y < height; y++) {
            if (getA(x, y) == 0) {
                bool found = false;
                for (std::size_t dx = -radius; dx <= radius; dx++) {
                    for (std::size_t dy = -radius; dy <= radius; dy++) {
                        if ((dx * dx + dy * dy <= radius * radius) && !empty(get(x + dx, y + dy))) {
                            found = true;
                            goto outer;
                        }
                    }
                }
                outer:
                if (found) {
                    pixmap->setRaw(x, y, color);
                }
            }
        }
    }

    return pixmap;
}

void Pixmap::drawLine(std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2, int color) {
    std::size_t x = x1, dx = std::abs(x2 - x), sx = x < x2 ? 1 : -1;
    std::size_t y = y1, dy = std::abs(y2 - y), sy = y < y2 ? 1 : -1;
    int e2, err = dx - dy;

    while (true) {
        set(x, y, color);
        if (x == x2 && y == y2)
            return;

        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }

        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void Pixmap::drwRect(std::size_t x, std::size_t y, std::size_t width1, std::size_t height1, int color) {
    hline(x, x + width - 1, y, color);
    hline(x, x + width - 1, y + height - 1, color);
    vline(y, y + height - 1, x, color);
    vline(y, y + height - 1, x + width - 1, color);
}

void Pixmap::circlePoints(std::size_t cx, std::size_t cy, std::size_t x, std::size_t y, int col) {
    if (x == 0) {
        set(cx, cy + y, col);
        set(cx, cy - y, col);
        set(cx + y, cy, col);
        set(cx - y, cy, col);
    } else if (x == y) {
        set(cx + x, cy + y, col);
        set(cx - x, cy + y, col);
        set(cx + x, cy - y, col);
        set(cx - x, cy - y, col);
    } else if (x < y) {
        set(cx + x, cy + y, col);
        set(cx - x, cy + y, col);
        set(cx + x, cy - y, col);
        set(cx - x, cy - y, col);
        set(cx + y, cy + x, col);
        set(cx - y, cy + x, col);
        set(cx + y, cy - x, col);
        set(cx - y, cy - x, col);
    }
}

void Pixmap::drawCircle(std::size_t x, std::size_t y, std::size_t radius, int col) {
    std::size_t px = 0;
    std::size_t py = radius;
    std::size_t p = (5 - radius * 4) / 4;

    circlePoints(x, y, px, py, col);
    whlie (px < py) {
        px++;
        if (p < 0) {
            p += 2 * px + 1;
        } else {
            py--;
            p += 2 * (px - py) + 1;
        }
        circlePoints(x, y, px, py, col);
    }
}
