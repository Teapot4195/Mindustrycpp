//
// Created by teapot on 23/11/23.
//

#include "Position.h"

#include <math/Angles.h>
#include <math/Mathf.h>

float Position::angleTo(const std::shared_ptr<Position> &other) {
    return Angles::angle(getX(), getY(), other->getX(), other->getY());
}

float Position::angleTo(float x, float y) {
    return Angles::angle(getX(), getY(), x, y);
}

float Position::dst2(const std::shared_ptr<Position> &other) {
    return dst2(other->getX(), other->getY());
}

float Position::dst(const std::shared_ptr<Position> &other) {
    return dst(other->getX(), other->getY());
}

float Position::dst(float x, float y) {
    const float xd = getX() - x;
    const float yd = getY() - y;
    return Mathf::sqrt(xd * xd + yd * yd);
}

float Position::dst2(float x, float y) {
    const float xd = getX() - x;
    const float yd = getY() - y;
    return (xd * xd + yd * yd);
}

bool Position::within(const std::shared_ptr<Position> &other, float dst) {
    return within(other->getX(), other->getY(), dst);
}

bool Position::within(float x, float y, float dst) {
    return Mathf::dst2(getX(), getY(), x, y) < dst*dst;
}
