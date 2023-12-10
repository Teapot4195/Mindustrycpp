//
// Created by teapot on 23/11/23.
//

#include "Angles.h"

#include <arc/Core.h>
#include "Mathf.h"
#include "math/geom/Vec2.h"

Rand Angles::rand {};

std::shared_ptr<Vec2> Angles::rv = std::make_shared<Vec2>();

float Angles::forwardDistance(float angle1, float angle2) {
    return std::abs(angle1 - angle2);
}

float Angles::backwardDistance(float angle1, float angle2) {
    return 360 - std::abs(angle1 - angle2);
}

bool Angles::within(float a, float b, float margin) {
    return angleDist(a, b) <= margin;
}

float Angles::angleDist(float a, float b) {
    a = Mathf::mod(a, 360.f);
    b = Mathf::mod(b, 360.f);
    return std::min((a - b) < 0 ? a - b + 360 : a - b, (b - a) < 0 ? b - a + 360 : b - a);
}

bool Angles::near(float a, float b, float range) {
    return angleDist(a, b) < range;
}

float Angles::clampRange(float angle, float dest, float range) {
    float dst = angleDist(angle, dest);
    return dst <= range ? angle : moveToward(angle, dest, dst - range);
}

float Angles::moveToward(float angle, float to, float speed) {
    if (std::abs(angleDist(angle, to)) < speed)
        return to;
    angle = Mathf::mod(angle, 360.f);
    to = Mathf::mod(to, 360.f);

    if (angle > to == backwardDistance(angle, to) > forwardDistance(angle, to))
        angle -= speed;
    else
        angle += speed;

    return angle;
}

float Angles::angle(float x, float y) {
    return angle(0, 0, x, y);
}

float Angles::angle(float x, float y, float x2, float y2) {
    float ang = Mathf::atan2(x2 - x, y2 - y) * Mathf::radDeg;
    if (ang < 0)
        ang += 360.f;
    return ang;
}

float Angles::angleRad(float x, float y, float x2, float y2) {
    return Mathf::atan2(x2 - x, y2 - y);
}

float Angles::trnsx(float angle, float len) {
    return len * Mathf::cosDeg(angle);
}

float Angles::trnsy(float angle, float len) {
    return len * Mathf::sinDeg(angle);
}

float Angles::trnsx(float angle, float x, float y) {
    return rv->set(x, y)->rotate(angle)->x;
}

float Angles::trnsy(float angle, float x, float y) {
    return rv->set(x, y)->rotate(angle)->y;
}

float Angles::mouseAngle(float cx, float cy) {
    std::shared_ptr<Vec2> avector = Core::camera->project(cx, cy);
    return angle(avector->x, avector->y, Core::input.mouseX(), Core::input.mouseY());
}

void Angles::circleVectors(std::uint32_t points, float length, const Floatc2& pos) {
    for (int i = 0; i < points; i++) {
        float f = (float)i * 360 / (float)points;
        pos(trnsx(f, length), trnsy(f, length));
    }
}

void Angles::circleVectors(std::uint32_t points, float length, float offset, const Floatc2 &pos) {
    for (int i = 0; i < points; i++) {
        float f = (float)i * 360 / (float)points + offset;
        pos(trnsx(f, length), trnsy(f, length));
    }
}

void Angles::randVectors(std::uint64_t seed, std::uint32_t amount, float length, const Floatc2 &cons) {
    rand.setSeed(seed);
    for (int i = 0; i < amount; i++) {
        rv->trns(rand.random(360.f), length);
        cons(rv->x, rv->y);
    }
}

void Angles::randLenVectors(std::uint64_t seed, std::uint32_t amount, float length, const Floatc2 &cons) {
    rand.setSeed(seed);
    for (int i = 0; i < amount; i++) {
        rv->trns(rand.random(360.f), rand.random(length));
        cons(rv->x, rv->y);
    }
}

void Angles::randLenVectors(std::uint64_t seed, std::uint32_t amount, float minLength, float length, const Floatc2 &cons) {
    rand.setSeed(seed);
    for (int i = 0; i < amount; i++) {
        rv->trns(rand.random(360.f), minLength + rand.random(length));
        cons(rv->x, rv->y);
    }
}

void Angles::randLenVectors(std::uint64_t seed, std::uint32_t amount, float length, float angle, float range, const Floatc2 &cons) {
    rand.setSeed(seed);
    for (int i = 0; i < amount; i++) {
        rv-> trns(angle + rand.range(range), rand.random(length));
        cons(rv->x, rv->y);
    }
}

void Angles::randLenVectors(std::uint64_t seed, std::uint32_t amount, float length, float angle, float range, float spread,
                            const Floatc2 &cons) {
    rand.setSeed(seed);
    for (int i = 0; i < amount; i++) {
        rv->trns(angle + rand.range(range), rand.random(length));
        cons(rv->x + rand.range(spread), rv->y + rand.range(spread));
    }
}

void Angles::randLenVectors(std::uint64_t seed, float fin, std::uint32_t amount, float length, const Angles::ParticleConsumer &cons) {
    rand.setSeed(seed);
    for (int i = 0; i < amount; i++) {
        float l = rand.nextFloat();
        rv->trns(rand.random(360.f), length * l * fin);
        cons(rv->x, rv->y, fin * l, (1.f - fin) * l);
    }
}

void Angles::randLenVectors(std::uint64_t seed, float fin, std::uint32_t amount, float length, float angle, float range,
                            const Angles::ParticleConsumer &cons) {
    rand.setSeed(seed);
    for (int i = 0; i < amount; i++) {
        rv->trns(angle + rand.range(range), rand.random(length * fin));
        cons(rv->x, rv->y, fin * (rand.nextFloat()), 0.f);
    }
}
