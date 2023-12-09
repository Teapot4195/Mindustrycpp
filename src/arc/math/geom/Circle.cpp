//
// Created by teapot on 09/12/23.
//

#include "Circle.h"

#include <arc/math/Mathf.h>
#include <arc/math/geom/Vec2.h>

Circle::Circle() = default;

Circle::Circle(float x, float y, float radius) : x(x), y(y), radius(radius) {}

Circle::Circle(const std::shared_ptr<Vec2> &position, float radius) : x(position->x), y(position->y), radius(radius) {}

Circle::Circle(const Circle &circle) : x(circle.x), y(circle.y), radius(circle.radius) {}

Circle::Circle(const std::shared_ptr<Circle> &circle) : x(circle->x), y(circle->y), radius(circle->radius) {}

Circle::Circle(const std::shared_ptr<Vec2>& center, const std::shared_ptr<Vec2>& edge) : x(center->x), y(center->y) {
    radius = Mathf::len(center->x - edge->x, center->y - edge->y);
}

std::shared_ptr<Circle> Circle::set(float x1, float y1, float radius1) {
    x = x1;
    y = y1;
    radius = radius1;

    return shared_from_this();
}

std::shared_ptr<Circle> Circle::set(const std::shared_ptr<Vec2>& position, float radius1) {
    x = position->x;
    y = position->y;
    radius = radius1;

    return shared_from_this();
}

std::shared_ptr<Circle> Circle::set(const std::shared_ptr<Circle> &circle) {
    x = circle->x;
    y = circle->y;
    radius = circle->radius;
    return shared_from_this();
}

std::shared_ptr<Circle> Circle::set(const std::shared_ptr<Vec2> &center, const std::shared_ptr<Vec2> &edge) {
    x = center->x;
    y = center->y;
    radius = Mathf::len(center->x - edge->x, center->y - edge->y);
    return shared_from_this();
}

std::shared_ptr<Circle> Circle::setPosition(const std::shared_ptr<Vec2> &position) {
    x = position->x;
    y = position->y;
    return shared_from_this();
}

std::shared_ptr<Circle> Circle::setPosition(float x1, float y1) {
    x = x1;
    y = y1;
    return shared_from_this();
}

void Circle::setX(float x1) {
    x = x1;
}

void Circle::setY(float y1) {
    y = y1;
}

void Circle::setRadius(float radius1) {
    radius = radius1;
}

bool Circle::contains(float x1, float y1) {
    x1 = x - x1;
    y1 = y - y1;
    return x1 * x1 + y1 * y1 <= radius * radius;
}

bool Circle::contains(std::shared_ptr<Vec2> point) {
    float dx = x - point->x;
    float dy = y - point->y;
    return dx * dx + dy * dy <= radius * radius;
}

bool Circle::contains(const std::shared_ptr<Circle>& c) const {
    float radiusDiff = radius - c->radius;
    if (radiusDiff < 0)
        return false;
    float dx = x - c->x;
    float dy = y - c->y;
    float dst = dx * dx + dy * dy;
    float radiusSum = radius + c->radius;
    return (radiusDiff * radiusDiff >= dst && (dst < radiusSum * radiusSum));
}

bool Circle::overlaps(const std::shared_ptr<Circle> &c) const {
    float dx = x - c->x;
    float dy = y - c->y;
    float distance = dx * dx + dy * dy;
    float radiusSum = radius + c->radius;
    return distance < radiusSum * radiusSum;
}

std::string Circle::toString() {
    return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(radius);
}

float Circle::circumference() const {
    return radius * Mathf::PI2;
}

float Circle::area() const {
    return radius * radius * Mathf::PI;
}

bool Circle::operator==(std::shared_ptr<Circle> &o) {
    if (o.get() == this)
        return true;
    if (o)
        return false;
    return x == o->x && y == o->y && radius == o->radius;
}
