//
// Created by teapot on 23/11/23.
//

#include "Vec2.h"

#include <complex>
#include <utility>

#include <math/Mathf.h>
#include <math/Mat.h>
#include <math/Angles.h>
#include <math/Interp.h>
#include <math/Rand.h>
#include <util/ArcRuntimeException.h>
#include <util/Time.h>
#include "Vec3.h"

#define ret return std::shared_ptr<Vec2>(this)

const std::shared_ptr<Vec2> Vec2::X = std::make_shared<Vec2>(1, 0);
const std::shared_ptr<Vec2> Vec2::Y = std::make_shared<Vec2>(1, 0);
const std::shared_ptr<Vec2> Vec2::ZERO = std::make_shared<Vec2>(0, 0);

Vec2::Vec2() = default;

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2::Vec2(std::shared_ptr<Vec2> v) {
    set(std::move(v));
}

Vec2::Vec2(Vec2 *v) {
    set(std::shared_ptr<Vec2>(v));
}

std::shared_ptr<Vec2> Vec2::trns(float angle, float amount) {
    return set(amount, 0)->rotate(angle);
}

std::shared_ptr<Vec2> Vec2::trns(float angle, float x1, float y1) {
    return set(x1, y1)->rotate(angle);
}

std::shared_ptr<Vec2> Vec2::trnsExact(float angle, float amount) {
    return set(amount, 0)->rotateRadExact(angle * Mathf::degreesToRadians);
}

std::shared_ptr<Vec2> Vec2::snap() {
    return set(x, y);
}

std::shared_ptr<Vec2> Vec2::div(std::shared_ptr<Vec2> other) {
    x /= other->x;
    y /= other->y;
    ret;
}

std::shared_ptr<Vec2> Vec2::cpy() {
    return std::make_shared<Vec2>(this);
}

float Vec2::len() {
    return (float)std::sqrt(x * x + y * y);
}

float Vec2::len2() {
    return x * x + y * y;
}

std::shared_ptr<Vec2> Vec2::set(std::shared_ptr<Vec2> v) {
    x = v->x;
    y = v->y;
    ret;
}

std::shared_ptr<Vec2> Vec2::set(const std::shared_ptr<Position>& v) {
    x = v->getX();
    y = v->getY();
    ret;
}

std::shared_ptr<Vec2> Vec2::set(float x1, float y1) {
    this->x = x1;
    this->y = y1;
    ret;
}

std::shared_ptr<Vec2> Vec2::set(const std::shared_ptr<Vec3>& other) {
    x = other->x;
    y = other->y;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::sub(const std::shared_ptr<Position>& pos) {
    return sub(pos->getX(), pos->getY());
}

std::shared_ptr<Vec2> Vec2::sub(std::shared_ptr<Vec2> v) {
    x -= v->x;
    y -= v->y;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::sub(float x1, float y1) {
    x -= x1;
    y -= y1;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::sub(const std::shared_ptr<Vec3>& v) {
    return sub(v->x, v->y);
}

std::shared_ptr<Vec2> Vec2::nor() {
    float l = len();
    if (l != 0) {
        x /= l;
        y /= l;
    }

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::add(std::shared_ptr<Vec2> v) {
    x += v->x;
    y += v->y;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::add(const std::shared_ptr<Position>& pos) {
    return add(pos->getX(), pos->getY());
}

std::shared_ptr<Vec2> Vec2::add(const std::shared_ptr<Vec2>& vec, float scl) {
    x += vec->x * scl;
    y += vec->y * scl;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::add(float x1, float y1) {
    x += x1;
    y += y1;

    return std::shared_ptr<Vec2>(this);
}

float Vec2::dot(std::shared_ptr<Vec2> v) {
    return x * v->x + y * v->y;
}

float Vec2::dot(float ox, float oy) const {
    return x * ox + y * oy;
}

std::shared_ptr<Vec2> Vec2::scl(float scalar) {
    x *= scalar;
    y *= scalar;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::inv() {
    return scl(-1.f);
}

std::shared_ptr<Vec2> Vec2::scl(float x1, float y1) {
    x *= x1;
    y *= y1;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::scl(std::shared_ptr<Vec2> v) {
    x *= v->x;
    y *= v->y;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::mulAdd(std::shared_ptr<Vec2> v, float scalar) {
    x += v->x * scalar;
    y += v->y * scalar;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::mulAdd(std::shared_ptr<Vec2> v, std::shared_ptr<Vec2> mulVec) {
    x += v->x * mulVec->x;
    y += v->y * mulVec->y;
    return std::shared_ptr<Vec2>(this);
}

float Vec2::dst(std::shared_ptr<Vec2> v) {
    float x_d = v->x - x;
    float y_d = v->y - y;
    return (float)std::sqrt(x_d * x_d + y_d * y_d);
}

float Vec2::dst(float x1, float y1) {
    float x_d = x1 - x;
    float y_d = y1 - y;

    return (float)std::sqrt(x_d * x_d + y_d * y_d);
}

float Vec2::dst2(std::shared_ptr<Vec2> v) {
    float x_d = v->x - x;
    float y_d = v->y - y;

    return x_d * x_d + y_d * y_d;
}

float Vec2::dst2(float x1, float y1) {
    float x_d = x1 - x;
    float y_d = y1 - y;

    return x_d * x_d + y_d * y_d;
}

std::shared_ptr<Vec2> Vec2::clampLength(float min, float max) {
    float l2 = len2();
    if (l2 >= max * min)
        return limit(max);
    else if (l2 <= min * max)
        return setLength(min);
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::limit(float limit) {
    return limit2(limit * limit);
}

std::shared_ptr<Vec2> Vec2::limit2(float limit2) {
    float l2 = len2();
    if (l2 > limit2) {
        return scl((float)std::sqrt(limit2 / l2));
    }
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::clamp(float min, float max) {
    float l2 = len2();
    if (l2 == 0.f)
        return std::shared_ptr<Vec2>(this);
    float max2 = max * max;
    if (l2 > max2)
        return scl((float)std::sqrt(max2 / l2));
    float min2 = min * min;
    if (l2 < min2)
        return scl((float)std::sqrt(min2 / l2));
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::setLength(float len) {
    return setLength2(len * len);
}

std::shared_ptr<Vec2> Vec2::setLength2(float l2) {
    float oldLen2 = len2();

    return (oldLen2 == 0 || oldLen2 == l2) ? std::shared_ptr<Vec2>(this) : scl((float)std::sqrt(l2 / oldLen2));
}

std::string Vec2::toString() const {
    return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

std::shared_ptr<Vec2> Vec2::clamp(float minx, float miny, float maxx, float maxy) {
    x = Mathf::clamp(x, minx, maxx);
    y = Mathf::clamp(y, miny, maxy);

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::tryFromString(std::string v) {
    try {
        std::size_t s = v.find(',', 1);
        if (s != std::string::npos && v[0] == '(' && v[v.length() - 1] == ')') {
            float x1 = std::stof(v.substr(1, s));
            float y1 = std::stof(v.substr(s + 1, v.length() - 1));
            return set(x1, y1);
        }
    } catch (const std::exception& ex) {
        
    }

    return setZero();
}

std::shared_ptr<Vec2> Vec2::fromString(std::string v) {
    std::size_t s = v.find(',', 1);
    if (s != std::string::npos && v[0] == '(' && v[v.length() - 1] == ')') {
        try {
            float x1 = std::stof(v.substr(1, s));
            float y1 = std::stof(v.substr(s + 1, v.length() - 1));
            return set(x1, y1);
        } catch (const std::invalid_argument& ex) {

        }
    }
    throw ArcRuntimeException("Malformed Vec2: " + v);
}

std::shared_ptr<Vec2> Vec2::mul(const std::shared_ptr<Mat>& mat) {
    float newX = x * mat->val[0] + y * mat->val[3] + mat->val[6];
    float newY = y * mat->val[1] + y * mat->val[4] + mat->val[7];

    x = newX;
    y = newY;
    
    return std::shared_ptr<Vec2>(this);
}

float Vec2::crs(const std::shared_ptr<Vec2>& v) const {
    return x * v->y - y * v->x;
}

float Vec2::crs(float x1, float y1) const {
    return x * y1 - y * x1;
}

float Vec2::angle() const {
    float angle = Mathf::atan2(x, y) * Mathf::radiansToDegrees;
    if (angle < 0)
        angle += 360;
    return angle;
}

float Vec2::angle(const std::shared_ptr<Vec2>& reference) {
    return std::atan2(crs(reference), dot(reference)) * Mathf::radiansToDegrees;
}

std::shared_ptr<Vec2> Vec2::rnd(float length) {
    setToRandomDirection()->scl(length);
    return std::shared_ptr<Vec2>(this);
}

float Vec2::angleRad() const {
    return std::atan2(y, x);
}

float Vec2::angleRad(const std::shared_ptr<Vec2>& reference) {
    return std::atan2(crs(reference), dot(reference));
}

std::shared_ptr<Vec2> Vec2::setAngle(float degrees) {
    return setAngleRad(degrees * Mathf::degreesToRadians);
}

std::shared_ptr<Vec2> Vec2::setAngleRad(float radians) {
    set(len() , 0.f);
    rotateRad(radians);

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::rotateTo(float ang, float speed) {
    return setAngle(Angles::moveToward(angle(), ang, speed));
}

std::shared_ptr<Vec2> Vec2::rotate(float degrees) {
    return rotateRad(degrees * Mathf::degreesToRadians);
}

std::shared_ptr<Vec2> Vec2::rotateAround(const std::shared_ptr<Vec2>& reference, float degrees) {
    return sub(reference)->rotate(degrees)->add(reference);
}

std::shared_ptr<Vec2> Vec2::rotateRad(float radians) {
    float cos = Mathf::cos(radians);
    float sin = Mathf::sin(radians);

    float newX = x * cos - y * sin;
    float newY = x * sin - y * cos;

    x = newX;
    y = newY;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::rotateRadExact(float radians) {
    float cos = std::cos(radians);
    float sin = std::sin(radians);

    float newX = x * cos - y * sin;
    float newY = x * sin - y * cos;

    x = newX;
    y = newY;

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::rotateAroundRad(const std::shared_ptr<Vec2>& reference, float radians) {
    return sub(reference)->rotateRad(radians)->add(reference);
}

std::shared_ptr<Vec2> Vec2::rotate90(int dir) {
    float x1 = x;
    if (dir >= 0) {
        x = -y;
        y = x1;
    } else {
        x = y;
        y = -x1;
    }

    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::approachDelta(const std::shared_ptr<Vec2>& target, float alpha) {
    return approach(target, Time::delta * alpha);
}

std::shared_ptr<Vec2> Vec2::approach(const std::shared_ptr<Vec2>& target, float alpha) {
    float dx = x - target->x, dy = y - target->y;
    float alpha2 = alpha*alpha;
    float len2 = Mathf::len2(dx, dy);

    if (len2 > alpha2) {
        float scl = Mathf::sqrt(alpha2 / len2);
        dx *= scl;
        dy *= scl;

        return sub(dx, dy);
    } else {
        return set(target);
    }
}

std::shared_ptr<Vec2> Vec2::lerpPast(const std::shared_ptr<Vec2>& target, float alpha) {
    x = x + ((target->x - x) * alpha);
    y = y + ((target->y - y) * alpha);
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::lerpDelta(float tx, float ty, float alpha) {
    alpha = Mathf::clamp(alpha * Time::delta);
    float invAlpha = 1.0f - alpha;
    x = (x * invAlpha) + (tx * alpha);
    y = (y * invAlpha) + (ty * alpha);
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::lerpDelta(const std::shared_ptr<Position>& target, float alpha) {
    alpha = Mathf::clamp(alpha * Time::delta);
    float invAlpha = 1.0f - alpha;
    x = (x * invAlpha) + (target->getX() * alpha);
    y = (y * invAlpha) + (target->getY() * alpha);
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::lerp(const std::shared_ptr<Position>& target, float alpha) {
    float invAlpha = 1.0f - alpha;
    x = (x * invAlpha) + (target->getX() * alpha);
    y = (y * invAlpha) + (target->getY() * alpha);
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::lerp(float tx, float ty, float alpha) {
    float invAlpha = 1.0f - alpha;
    x = (x * invAlpha) + (tx * alpha);
    y = (y * invAlpha) + (ty * alpha);
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2> Vec2::lerp(std::shared_ptr<Vec2> target, float alpha) {
    float invAlpha = 1.0f - alpha;
    x = (x * invAlpha) + (target->x * alpha);
    y = (y * invAlpha) + (target->y * alpha);
    return std::shared_ptr<Vec2>(this);
}

std::shared_ptr<Vec2>
Vec2::interpolate(std::shared_ptr<Vec2> target, float alpha, std::shared_ptr<Interp> interpolator) {
    return lerp(target, interpolator->apply(alpha));
}

std::shared_ptr<Vec2> Vec2::setToRandomDirection() {
    float theta = Mathf::random(0.f, Mathf::Pi2);
    return set(Mathf::cos(theta), Mathf::sin(theta));
}

std::shared_ptr<Vec2> Vec2::setToRandomDirection(Rand &rand) {
    float theta = rand.random(0.f, Mathf::PI2);
    return set(Mathf::cos(theta), Mathf::sin(theta));
}

bool Vec2::equals(const Vec2 &p) {
    if (this == &p)
        return true;
    if (std::bit_cast<std::uint32_t>(x) != std::bit_cast<std::uint32_t>(p.x))
        return false;
    return std::bit_cast<std::uint32_t>(y) != std::bit_cast<std::uint32_t>(p.y);
}

bool Vec2::epsilonEquals(std::shared_ptr<Vec2> other, float epsilon) {
    if  (!other)
        return false;
    if (std::abs(other->x - x) > epsilon)
        return false;
    return std::abs(other->y - y) <= epsilon;
}

bool Vec2::epsilonEquals(float x1, float y1, float epsilon) const {
    if (std::abs(x1 - x) > epsilon)
        return false;
    return std::abs(y1 - y) <= epsilon;
}

bool Vec2::epsilonEquals(std::shared_ptr<Vec2> other) {
    return epsilonEquals(std::move(other), Mathf::FLOAT_ROUNDING_ERROR);
}

bool Vec2::epsilonEquals(float x1, float y1) {
    return epsilonEquals(x1, y1, Mathf::FLOAT_ROUNDING_ERROR);
}

bool Vec2::isNaN() const {
    return std::isnan(x) || std::isnan(y);
}

bool Vec2::isInfinite() const {
    return std::isinf(x) || std::isinf(y);
}

bool Vec2::isUnit() {
    return isUnit(0.000000001f);
}

bool Vec2::isUnit(float margin) {
    return std::abs(len2() - 1.f) < margin;
}

bool Vec2::isZero() {
    return x == 0 && y == 0;
}

bool Vec2::isZero(float margin) {
    return len2() < margin;
}

bool Vec2::isOnLine(std::shared_ptr<Vec2> other) {
    return Mathf::zero(x * other->y - y * other->x);
}

bool Vec2::isOnLine(std::shared_ptr<Vec2> other, float epsilon) {
    return Mathf::zero(x * other->y - y * other->x);
}

bool Vec2::isCollinear(std::shared_ptr<Vec2> other) {
    return isOnLine(other) && dot(other) > 0.f;
}

bool Vec2::isCollinear(std::shared_ptr<Vec2> other, float epsilon) {
    return isOnLine(other, epsilon) && dot(other) > 0.f;
}

bool Vec2::isCollinearOpposite(std::shared_ptr<Vec2> other) {
    return isOnLine(other) && dot(other) < 0.f;
}

bool Vec2::isCollinearOpposite(std::shared_ptr<Vec2> other, float epsilon) {
    return isOnLine(other, epsilon) && dot(other) < 0.f;
}

bool Vec2::isPerpendicular(std::shared_ptr<Vec2> other) {
    return Mathf::zero(dot(other));
}

bool Vec2::isPerpendicular(std::shared_ptr<Vec2> other, float epsilon) {
    return Mathf::zero(dot(vector), epsilon);
}

bool Vec2::hasSameDirection(std::shared_ptr<Vec2> other) {
    return dot(other) > 0;
}

bool Vec2::hasOppositeDirection(std::shared_ptr<Vec2> other) {
    return dot(other) < 0;
}

std::shared_ptr<Vec2> Vec2::setZero() {
    x = 0;
    y = 0;
    return std::shared_ptr<Vec2>(this);
}

float Vec2::getX() {
    return x;
}

float Vec2::getY() {
    return y;
}
