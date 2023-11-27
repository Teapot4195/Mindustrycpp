//
// Created by teapot on 23/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_VECTOR_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_VECTOR_H

#include <memory>

// Forward decl to resolve circular reference
class Interp;

template <typename T>
class Vector {
public:
    virtual ~Vector() = default;

    virtual std::shared_ptr<T> cpy() = 0;

    virtual float len() = 0;

    virtual float len2() = 0;

    virtual std::shared_ptr<T> limit(float limit) = 0;

    virtual std::shared_ptr<T> limit2(float limit2) = 0;

    virtual std::shared_ptr<T> setLength(float len) = 0;

    virtual std::shared_ptr<T> setLength2(float len2) = 0;

    virtual std::shared_ptr<T> clamp(float min, float max) = 0;

    virtual std::shared_ptr<T> set(std::shared_ptr<T> v) = 0;

    virtual std::shared_ptr<T> sub(std::shared_ptr<T> v) = 0;

    virtual std::shared_ptr<T> nor() = 0;

    virtual std::shared_ptr<T> add(std::shared_ptr<T> v) = 0;

    virtual float dot(std::shared_ptr<T> v) = 0;

    virtual std::shared_ptr<T> scl(float scalar) = 0;

    virtual std::shared_ptr<T> scl(std::shared_ptr<T> v) = 0;

    virtual std::shared_ptr<T> div(std::shared_ptr<T> other) = 0;

    virtual float dst(std::shared_ptr<T> v) = 0;

    virtual float dst2(std::shared_ptr<T> v) = 0;

    virtual std::shared_ptr<T> lerp(std::shared_ptr<T> target, float alpha) = 0;

    virtual std::shared_ptr<T> interpolate(std::shared_ptr<T> target, float alpha, std::shared_ptr<Interp> interpolator) = 0;

    virtual std::shared_ptr<T> setToRandomDirection() = 0;

    virtual bool isUnit() = 0;

    virtual bool isUnit(float margin) = 0;

    virtual bool isZero() = 0;

    virtual bool isZero(float margin) = 0;

    virtual bool isOnLine(std::shared_ptr<T> other, float epsilon) = 0;

    virtual bool isOnLine(std::shared_ptr<T> other) = 0;

    virtual bool isCollinear(std::shared_ptr<T> other, float epsilon) = 0;

    virtual bool isCollinear(std::shared_ptr<T> other) = 0;

    virtual bool isCollinearOpposite(std::shared_ptr<T> other, float epsilon) = 0;

    virtual bool isCollinearOpposite(std::shared_ptr<T> other) = 0;

    virtual bool isPerpendicular(std::shared_ptr<T> other) = 0;

    virtual bool isPerpendicular(std::shared_ptr<T> other, float epsilon) = 0;

    virtual bool hasSameDirection(std::shared_ptr<T> other) = 0;

    virtual bool hasOppositeDirection(std::shared_ptr<T> other) = 0;

    virtual bool epsilonEquals(std::shared_ptr<T> other, float epsilon) = 0;

    virtual std::shared_ptr<T> mulAdd(std::shared_ptr<T> v, float scalar) = 0;

    virtual std::shared_ptr<T> mulAdd(std::shared_ptr<T> v, std::shared_ptr<T> mulVec) = 0;

    virtual std::shared_ptr<T> setZero() = 0;

    virtual std::shared_ptr<T> plus(std::shared_ptr<T> other) {
        return add(other);
    }

    virtual std::shared_ptr<T> minus(std::shared_ptr<T> other) {
        return sub(other);
    }

    virtual std::shared_ptr<T> unaryMinus() {
        return scl(-1);
    }

    virtual std::shared_ptr<T> times(std::shared_ptr<T> other) {
        return scl(other);
    }
};


#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_VECTOR_H
