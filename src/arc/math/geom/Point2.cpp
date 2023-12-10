//
// Created by teapot on 09/12/23.
//

#include "Point2.h"

#include <cmath>

Point2::Point2() = default;

Point2::Point2(int x1, int y1) : x_(x1), y_(y1) {}

Point2::Point2(const std::shared_ptr<Point2>& point) : x_(point->x_), y_(point->y_) {}

std::shared_ptr<Point2> Point2::unpack(int pos) {
    return std::make_shared<Point2>((short)(pos >> 16), (short)(pos & 0xFFFF));
}

int Point2::pack(int x, int y) {
    return (((short)x) << 16) | (((short)y) & 0xFFFF);
}

short Point2::x(int pos) {
    return (short)(pos >> 16);
}

short Point2::y(int pos) {
    return (short)(pos & 0xFFFF);
}

int Point2::pack() const {
    return pack(x_, y_);
}

std::shared_ptr<Point2> Point2::set(const std::shared_ptr<Point2>& point) {
    x_ = point->x_;
    y_ = point->y_;
    return shared_from_this();
}

std::shared_ptr<Point2> Point2::set(int x, int y) {
    x_ = x;
    y_ = y;
    return shared_from_this();
}

float Point2::dst2(const std::shared_ptr<Point2>& other) const {
    float xd = (float)other->x_ - (float)x_;
    float yd = (float)other->y_ - (float)y_;

    return xd * xd + yd * yd;
}

float Point2::dst2(int x, int y) const {
    float xd = (float)x_ - (float)x;
    float yd = (float)y_ - (float)y;

    return xd * xd + yd * yd;
}

float Point2::dst(const std::shared_ptr<Point2> &other) const {
    return std::sqrt(dst2(other));
}

float Point2::dst(int x, int y) const {
    return std::sqrt(dst2(x, y));
}

std::shared_ptr<Point2> Point2::add(const std::shared_ptr<Point2> &other) {
    x_ += other->x_;
    y_ += other->y_;
    return shared_from_this();
}

std::shared_ptr<Point2> Point2::add(int x, int y) {
    x_ += x;
    y_ += y;

    return shared_from_this();
}

std::shared_ptr<Point2> Point2::sub(const std::shared_ptr<Point2> &other) {
    x_ -= other->x_;
    y_ -= other->y_;

    return shared_from_this();
}

std::shared_ptr<Point2> Point2::sub(int x, int y) {
    x_ -= x;
    y_ -= y;

    return shared_from_this();
}

std::shared_ptr<Point2> Point2::cpy() {
    return std::make_shared<Point2>(shared_from_this());
}

std::shared_ptr<Point2> Point2::rotate(int steps) {
    for (int i = 0; i < std::abs(steps); i++) {
        auto x = x_;
        if (steps >= 0) {
            x_ = -y_;
            y_ = x;
        } else {
            x_ = y_;
            y_ = -x;
        }
    }
    return shared_from_this();
}

bool Point2::equals(int x, int y) const {
    return x_ == x && y_ == y;
}

bool Point2::equals(int x, int y, int ox, int oy) {
    return x == ox && y == oy;
}

bool Point2::operator==(const std::shared_ptr<Point2> &g) const {
    if (this == g.get())
        return true;
    if (g)
        return false;
    return x_ == g->x_ && y_ == g->y_;
}

std::string Point2::toString() {
    return "(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
}
