//
// Created by teapot on 23/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_VEC2_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_VEC2_H

#include <cstdint>

#include "Position.h"
#include "Vector.h"

// Forward decl to resolve circular dependency
class Vec3;
class Mat;
class Rand;

class Vec2 : public Vector<Vec2>, public Position {
public:
    static const std::shared_ptr<Vec2> X, Y, ZERO;

    float x{}, y{};

    Vec2();

    Vec2(float x, float y);

    explicit Vec2(std::shared_ptr<Vec2> v);

    explicit Vec2(Vec2* v);

    std::shared_ptr<Vec2> trns(float angle, float amount);

    std::shared_ptr<Vec2> trns(float angle, float x1, float y1);

    std::shared_ptr<Vec2> trnsExact(float angle, float amount);

    std::shared_ptr<Vec2> snap();

    std::shared_ptr<Vec2> div(std::shared_ptr<Vec2> other) override;

    std::shared_ptr<Vec2> cpy() override;

    float len() override;

    float len2() override;

    std::shared_ptr<Vec2> set(std::shared_ptr<Vec2> v) override;

    std::shared_ptr<Vec2> set(const std::shared_ptr<Position>& v);

    std::shared_ptr<Vec2> set(float x1, float y1);

    std::shared_ptr<Vec2> set(const std::shared_ptr<Vec3>& other);

    std::shared_ptr<Vec2> sub(const std::shared_ptr<Position>& pos);

    std::shared_ptr<Vec2> sub(std::shared_ptr<Vec2> v) override;

    std::shared_ptr<Vec2> sub(float x1, float y1);

    std::shared_ptr<Vec2> sub(const std::shared_ptr<Vec3>& v);

    std::shared_ptr<Vec2> nor() override;

    std::shared_ptr<Vec2> add(std::shared_ptr<Vec2> v) override;

    std::shared_ptr<Vec2> add(const std::shared_ptr<Position>& pos);

    std::shared_ptr<Vec2> add(const std::shared_ptr<Vec2>& vec, float scl);

    std::shared_ptr<Vec2> add(float x1, float y1);

    float dot(std::shared_ptr<Vec2> v) override;

    float dot(float ox, float oy) const;

    std::shared_ptr<Vec2> scl(float scalar) override;

    std::shared_ptr<Vec2> inv();

    std::shared_ptr<Vec2> scl(float x1, float y1);

    std::shared_ptr<Vec2> scl(std::shared_ptr<Vec2> v) override;

    std::shared_ptr<Vec2> mulAdd(std::shared_ptr<Vec2> v, float scalar) override;

    std::shared_ptr<Vec2> mulAdd(std::shared_ptr<Vec2> v, std::shared_ptr<Vec2> mulVec) override;

    float dst(std::shared_ptr<Vec2> v) override;

    float dst(float x1, float y1);

    float dst2(std::shared_ptr<Vec2> v) override;

    float dst2(float x1, float y1) override;

    std::shared_ptr<Vec2> clampLength(float min, float max);

    std::shared_ptr<Vec2> limit(float limit) override;

    std::shared_ptr<Vec2> limit2(float limit2) override;

    std::shared_ptr<Vec2> clamp(float min, float max) override;

    std::shared_ptr<Vec2> setLength(float len) override;

    std::shared_ptr<Vec2> setLength2(float l2) override;

    std::string toString() const;

    std::shared_ptr<Vec2> clamp(float minx, float miny, float maxx, float maxy);

    std::shared_ptr<Vec2> tryFromString(std::string v);

    std::shared_ptr<Vec2> fromString(std::string v);

    std::shared_ptr<Vec2> mul(const std::shared_ptr<Mat>& mat);

    float crs(const std::shared_ptr<Vec2>& v) const;

    float crs(float x1, float y1) const;

    float angle() const;

    float angle(const std::shared_ptr<Vec2>& reference);

    std::shared_ptr<Vec2> rnd(float length);

    float angleRad() const;

    float angleRad(const std::shared_ptr<Vec2>& reference);

    std::shared_ptr<Vec2> setAngle(float degrees);

    std::shared_ptr<Vec2> setAngleRad(float radians);

    std::shared_ptr<Vec2> rotateTo(float ang, float speed);

    std::shared_ptr<Vec2> rotate(float degrees);

    std::shared_ptr<Vec2> rotateAround(const std::shared_ptr<Vec2>& reference, float degrees);

    std::shared_ptr<Vec2> rotateRad(float radians);

    std::shared_ptr<Vec2> rotateRadExact(float radians);

    std::shared_ptr<Vec2> rotateAroundRad(const std::shared_ptr<Vec2>& reference, float radians);

    std::shared_ptr<Vec2> rotate90(int dir);

    std::shared_ptr<Vec2> approachDelta(const std::shared_ptr<Vec2>& target, float alpha);

    std::shared_ptr<Vec2> approach(const std::shared_ptr<Vec2>& target, float alpha);

    std::shared_ptr<Vec2> lerpPast(const std::shared_ptr<Vec2>& target, float alpha);

    std::shared_ptr<Vec2> lerpDelta(float tx, float ty, float alpha);

    std::shared_ptr<Vec2> lerpDelta(const std::shared_ptr<Position>& target, float alpha);

    std::shared_ptr<Vec2> lerp(const std::shared_ptr<Position>& target, float alpha);

    std::shared_ptr<Vec2> lerp(float tx, float ty, float alpha);

    std::shared_ptr<Vec2> lerp(std::shared_ptr<Vec2> target, float alpha) override;

    std::shared_ptr<Vec2> interpolate(std::shared_ptr<Vec2> target, float alpha, std::shared_ptr<Interp> interpolator) override;

    std::shared_ptr<Vec2> setToRandomDirection() override;

    std::shared_ptr<Vec2> setToRandomDirection(Rand& rand);

    bool equals(const Vec2 &p);

    bool epsilonEquals(std::shared_ptr<Vec2> other, float epsilon) override;

    bool epsilonEquals(float x1, float y1, float epsilon) const;

    bool epsilonEquals(std::shared_ptr<Vec2> other);

    bool epsilonEquals(float x1, float y1);

    bool isNaN() const;

    bool isInfinite() const;

    bool isUnit() override;

    bool isUnit(float margin) override;

    bool isZero() override;

    bool isZero(float margin) override;

    bool isOnLine(std::shared_ptr<Vec2> other) override;

    bool isOnLine(std::shared_ptr<Vec2> other, float epsilon) override;

    bool isCollinear(std::shared_ptr<Vec2> other) override;

    bool isCollinear(std::shared_ptr<Vec2> other, float epsilon) override;

    bool isCollinearOpposite(std::shared_ptr<Vec2> other) override;

    bool isCollinearOpposite(std::shared_ptr<Vec2> other, float epsilon) override;

    bool isPerpendicular(std::shared_ptr<Vec2> other) override;

    bool isPerpendicular(std::shared_ptr<Vec2> other, float epsilon) override;

    bool hasSameDirection(std::shared_ptr<Vec2> other) override;

    bool hasOppositeDirection(std::shared_ptr<Vec2> other) override;

    std::shared_ptr<Vec2> setZero() override;

    float getX() override;

    float getY() override;
};

template<>
struct std::hash<Vec2> {
    std::size_t operator()(const Vec2 &p) {
        std::size_t prime = 31;
        std::size_t result = 1;
        result = prime * result + std::bit_cast<std::uint32_t>(p.x);
        result = prime * result + std::bit_cast<std::uint32_t>(p.y);
        return result;
    }
};

#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_VEC2_H
