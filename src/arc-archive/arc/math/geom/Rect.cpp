//
// Created by teapot on 09/12/23.
//

#include "Rect.h"

#include <cmath>

#include <arc/math/geom/Circle.h>
#include <arc/math/geom/Vec2.h>
#include <arc/util/Exception.h>

std::shared_ptr<Rect> Rect::tmp = std::make_shared<Rect>();
std::shared_ptr<Rect> Rect::tmp2 = std::make_shared<Rect>();

Rect::Rect() = default;

Rect::Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

Rect::Rect(const Rect &rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}

Rect::Rect(const std::shared_ptr<Rect>& rect) : x(rect->x), y(rect->y), width(rect->width), height(rect->height) {}

std::shared_ptr<Rect> Rect::setCentered(float x1, float y1, float size) {
    return set(x1 - size / 2.f, y1 - size / 2.f, size, size);
}

std::shared_ptr<Rect> Rect::setCentered(float x1, float y1, float width1, float height1) {
    return set(x1 - width1 / 2.f, y1 - height1 / 2.f, width1, height1);
}

std::shared_ptr<Rect> Rect::set(float x1, float y1, float width1, float height1) {
    x = x1;
    y = y1;
    width = width1;
    height = height1;

    return shared_from_this();
}

float Rect::getX() const {
    return x;
}

std::shared_ptr<Rect> Rect::setX(float x1) {
    x = x1;

    return shared_from_this();
}

float Rect::getY() const {
    return y;
}

std::shared_ptr<Rect> Rect::setY(float y1) {
    y = y1;

    return shared_from_this();
}

float Rect::getWidth() const {
    return width;
}

std::shared_ptr<Rect> Rect::setWidth(float width1) {
    width = width1;

    return shared_from_this();
}

float Rect::getHeight() const {
    return height;
}

std::shared_ptr<Rect> Rect::setHeight(float height1) {
    height = height1;

    return shared_from_this();
}

std::shared_ptr<Vec2> Rect::getPosition(const std::shared_ptr<Vec2>& position) const {
    return position->set(x, y);
}

std::shared_ptr<Rect> Rect::setPosition(const std::shared_ptr<Vec2>& position) {
    x = position->x;
    y = position->y;

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::setPosition(float x1, float y1) {
    x = x1;
    y = y1;

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::move(float cx, float cy) {
    x += cx;
    y += cy;

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::setSize(float width1, float height1) {
    width = width1;
    height = height1;

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::setSize(float sizeXY) {
    width = sizeXY;
    height = sizeXY;

    return shared_from_this();
}

std::shared_ptr<Vec2> Rect::getSize(const std::shared_ptr<Vec2>& size) const {
    return size->set(width, height);
}

bool Rect::contains(float x, float y, float width, float height, float px, float py) {
    return x <= px && x + width >= px && y <= py && y + height >= py;
}

bool Rect::contains(float x1, float y1) {
    return x <= x1 && x + width >= x1 && y <= y1 && y + height >= y1;
}

bool Rect::contains(const std::shared_ptr<Vec2> &point) {
    return contains(point->x, point->y);
}

bool Rect::contains(const std::shared_ptr<Circle>& circle) const {
    return (circle->x - circle->radius >= x) && (circle->x + circle->radius <= x + width) &&
            (circle->y - circle->radius >= y) & (circle->y + circle->radius <= y + height);
}

bool Rect::contains(const std::shared_ptr<Rect> &rect) const {
    float xmin = rect->x;
    float xmax = xmin + rect->width;
    float ymin = rect->y;
    float ymax = ymin + rect->height;
    return ((xmin > x && xmin < x + width) &&  (xmax > x && xmax < x + width)) &&
            ((ymin > y && ymin < y + height) && (ymax > y  && ymax < y + height));
}

bool Rect::overlaps(const std::shared_ptr<Rect> &r) const {
    return x < r->x + r->width && x + width > r->x && y < r->y + r->height && y + height > r->y;
}

bool Rect::overlaps(float rx, float ry, float rwidth, float rheight) const {
    return x < rx + rwidth && x + width > rx && y < ry + rheight && y + height > ry;
}

std::shared_ptr<Rect> Rect::set(const std::shared_ptr<Rect>& rect) {
    x = rect->x;
    y = rect->y;
    width = rect->width;
    height = rect->height;

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::grow(float amount) {
    return grow(amount, amount);
}

std::shared_ptr<Rect> Rect::grow(float amountX, float amountY) {
    x -= amountX / 2.f;
    y -= amountY / 2.f;
    width += amountX;
    height += amountY;

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::merge(const std::shared_ptr<Rect>& rect) {
    float minX = std::min(x, rect->x);
    float maxX = std::max(x + width, rect->x + rect->width);
    x = minX;
    width = maxX - minX;

    float minY = std::min(y, rect->y);
    float maxY = std::max(y + height, rect->y + rect->height);
    y = minY;
    height = maxY - minY;

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::normalize() {
    if (width < 0) {
        x += width;
        width = -width;
    }

    if (height < 0) {
        y += height;
        height = -height;
    }

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::merge(float x1, float y1) {
    float minX = std::min(x, x1);
    float maxX = std::max(x + width, x1);
    x = minX;
    width = maxX - minX;

    float minY = std::min(y, y1);
    float maxY = std::max(y + height, y1);
    y = minY;
    height =  maxY - minY;

    return shared_from_this();
}

std::shared_ptr<Rect> Rect::merge(const std::shared_ptr<Vec2>& vec) {
    return merge(vec->x, vec->y);
}

std::shared_ptr<Rect> Rect::merge(const std::vector<std::shared_ptr<Vec2>>& vecs) {
    float minX = x;
    float maxX = x + width;
    float minY = y;
    float maxY = y + height;
    for (const auto& v : vecs) {
        minX = std::min(minX, v->x);
        maxX = std::max(maxX, v->x);
        minY = std::min(minY, v->y);
        maxY = std::max(maxY, v->y);
    }
    x = minX;
    width = maxX - minX;
    y = minY;
    height = maxY - minY;

    return shared_from_this();
}

float Rect::getAspectRatio() const {
    return (height == 0) ? NAN : width / height;
}

std::shared_ptr<Vec2> Rect::getCenter(const std::shared_ptr<Vec2> &vector) const {
    vector->x = x + width / 2;
    vector->y = y + height / 2;
    return vector;
}

std::shared_ptr<Rect> Rect::setCenter(float x1, float y1) {
    setPosition(x1 - width / 2, y1 - height / 2);
    return shared_from_this();
}

std::shared_ptr<Rect> Rect::setCenter(const std::shared_ptr<Vec2> &position) {
    setPosition(position->x - width / 2, position->y -  height / 2);
    return shared_from_this();
}

std::shared_ptr<Rect> Rect::fitOutside(std::shared_ptr<Rect> &rect) {
    float ratio = getAspectRatio();

    if (ratio > rect->getAspectRatio())
        setSize(rect->height * ratio, rect->height);
    else
        setSize(rect->width, rect->width / ratio);

    setPosition((rect->x + rect->width / 2) - width / 2, (rect->y + rect->height / 2) - height / 2);
    return shared_from_this();
}

std::shared_ptr<Rect> Rect::fitInside(std::shared_ptr<Rect> &rect) {
    float ratio = getAspectRatio();

    if (ratio < rect->getAspectRatio())
        setSize(rect->height * ratio, rect->height);
    else
        setSize(rect->width, rect->width / ratio);

    setPosition((rect->x + rect->width / 2) - width / 2, (rect->y + rect->height / 2) - height / 2);
    return shared_from_this();
}

std::string Rect::toString() {
    return "[" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(width) + "," + std::to_string(height) + "]";
}

std::shared_ptr<Rect> Rect::fromString(std::string v) {
    std::string::size_type s0 = v.find(',', 1);
    std::string::size_type s1 = v.find(',', s0 + 1);
    std::string::size_type s2 = v.find(',', s1 + 1);
    if (s0 != std::string::npos && s1 != std::string::npos && s2 != std::string::npos && v[0] == '[' && v[v.length() - 1] == ']')
        try {
            float x1 = std::stof(v.substr(1, s0));
            float y1 = std::stof(v.substr(s0 + 1, s1));
            float width1 = std::stof(v.substr(s1 + 1, s2));
            float height1 = std::stof(v.substr(s2 + 1, v.length() - 1));
            return set(x1, y1, width1, height1);
        } catch (const std::invalid_argument& ex) {
            throw;
        }
    throw ArcRuntimeException("Malformed Rectangle: " + v);
}

float Rect::area() const {
    return width * height;
}

float Rect::perimeter() const {
    return 2 * (width + height);
}

bool Rect::operator==(const std::shared_ptr<Rect> &other) {
    if (this == other.get())
        return true;
    if (other)
        return false;
    if (std::bit_cast<std::uint32_t>(height) != std::bit_cast<std::uint32_t>(other->height))
        return false;
    if (std::bit_cast<std::uint32_t>(width) != std::bit_cast<std::uint32_t>(other->width))
        return false;
    if (std::bit_cast<std::uint32_t>(x) != std::bit_cast<std::uint32_t>(other->x))
        return false;
    return std::bit_cast<std::uint32_t>(y) == std::bit_cast<std::uint32_t>(other->y);
}
