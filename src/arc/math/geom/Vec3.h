//
// Created by teapot on 23/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_VEC3_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_VEC3_H

#include <vector>

#include <math/Rand.h>
#include "Vector.h"

// Forward Decl to avoid possible future circular dependency
class Mat;
class Vec2;

class Vec3 : public Vector<Vec3> {
public:
    static const std::shared_ptr<Vec3> X;
    static const std::shared_ptr<Vec3> Y;
    static const std::shared_ptr<Vec3> Z;
    static const std::shared_ptr<Vec3> Zero;

private:
    static std::shared_ptr<Mat> tmpMat;

public:
    float x{}, y{}, z{};

    Vec3();

    Vec3(float x, float y, float z);

    explicit Vec3(const std::shared_ptr<Vec3>& vector);

    explicit Vec3(std::vector<float> values);

    Vec3(const std::shared_ptr<Vec2>& vector, float z);

    static float len(float x, float y, float z);

    static float len2(float x, float y, float z);

    static float dst(float x1, float y1, float z1, float x2, float y2, float z2);

    static float dst2(float x1, float y1, float z1, float x2, float y2, float z2);

    static float dot(float x1, float y1, float z1, float x2, float y2, float z2);

    std::shared_ptr<Vec3> set(float x1, float y1, float z1);

    std::shared_ptr<Vec3> div(std::shared_ptr<Vec3> other) override;

    std::shared_ptr<Vec3> set(std::shared_ptr<Vec3> v) override;

    std::shared_ptr<Vec3> set(std::vector<float> values);

    std::shared_ptr<Vec3> set(std::vector<float> values, std::size_t offset);

    std::shared_ptr<Vec3> set(const std::shared_ptr<Vec2> &vector, float z1);

    std::shared_ptr<Vec3> setFromSpherical(float azimuthalAngle, float polarAngle);

    std::shared_ptr<Vec3> setToRandomDirection() override;

    std::shared_ptr<Vec3> setToRandomDirection(Rand rand);

    std::shared_ptr<Vec3> cpy() override;

    std::shared_ptr<Vec3> add(std::shared_ptr<Vec3> v) override;

    std::shared_ptr<Vec3> cpy(const std::shared_ptr<Vec3>& dest);

    std::shared_ptr<Vec3> add(const std::shared_ptr<Vec3>& vector, float scale);

    std::shared_ptr<Vec3> sub(const std::shared_ptr<Vec3>& vector, float scale);

    std::shared_ptr<Vec3> add(float x1, float y1, float z1);

    std::shared_ptr<Vec3> sub(std::shared_ptr<Vec3> v) override;

    std::shared_ptr<Vec3> sub(float x1, float y1, float z1);

    std::shared_ptr<Vec3> sub(float value);

    std::shared_ptr<Vec3> scl(float scalar) override;

    std::shared_ptr<Vec3> scl(std::shared_ptr<Vec3> v) override;

    std::shared_ptr<Vec3> scl(float vx, float vy, float vz);

    std::shared_ptr<Vec3> mulAdd(std::shared_ptr<Vec3> v, float scalar) override;

    std::shared_ptr<Vec3> mulAdd(std::shared_ptr<Vec3> v, std::shared_ptr<Vec3> mulVec) override;

    float len() override;

    float len2() override;

    [[nodiscard]] bool idt(const std::shared_ptr<Vec3>& vector) const;

    float dst(std::shared_ptr<Vec3> v) override;

    float dst2(std::shared_ptr<Vec3> v) override;

    [[nodiscard]] float dst(float x1, float y1, float z1) const;

    [[nodiscard]] float dst2(float x1, float y1, float z1) const;

    bool within(std::shared_ptr<Vec3> v, float dst);

    std::shared_ptr<Vec3> nor() override;

    float dot(std::shared_ptr<Vec3> v) override;

    float angleRad(const std::shared_ptr<Vec3>& vector);

    float angle(const std::shared_ptr<Vec3>& vector);

    [[nodiscard]] float dot(float x1, float y1, float z1) const;

    std::shared_ptr<Vec3> crs(const std::shared_ptr<Vec3>& vector);

    std::shared_ptr<Vec3> crs(float x1, float y1, float z1);

    std::shared_ptr<Vec3> mul4x3(std::vector<float> matrix);

    std::shared_ptr<Vec3> mul(const std::shared_ptr<Mat>& matrix);

    std::shared_ptr<Vec3> traMul(const std::shared_ptr<Mat>& matrix);

    std::shared_ptr<Vec3> rotate(const std::shared_ptr<Vec3>& axis, float degrees);

    bool isUnit() override;

    bool isUnit(float margin) override;

    bool isZero() override;

    bool isZero(float margin) override;

    bool isOnLine(std::shared_ptr<Vec3> other) override;

    bool isOnLine(std::shared_ptr<Vec3> other, float epsilon) override;

    bool isCollinear(std::shared_ptr<Vec3> other) override;

    bool isCollinear(std::shared_ptr<Vec3> other, float epsilon) override;

    bool isCollinearOpposite(std::shared_ptr<Vec3> other, float epsilon) override;

    bool isCollinearOpposite(std::shared_ptr<Vec3> other) override;

    bool isPerpendicular(std::shared_ptr<Vec3> other) override;

    bool isPerpendicular(std::shared_ptr<Vec3> other, float epsilon) override;

    bool hasSameDirection(std::shared_ptr<Vec3> other) override;

    bool hasOppositeDirection(std::shared_ptr<Vec3> other) override;

    std::shared_ptr<Vec3> lerp(std::shared_ptr<Vec3> target, float alpha) override;

    std::shared_ptr<Vec3> interpolate(std::shared_ptr<Vec3> target, float alpha, std::shared_ptr<Interp> interpolator) override;

    std::shared_ptr<Vec3> slerp(const std::shared_ptr<Vec3>& target, float alpha);

    [[nodiscard]] std::string toString() const;

    std::shared_ptr<Vec3> fromString(std::string v);

    std::shared_ptr<Vec3> limit(float limit) override;

    std::shared_ptr<Vec3> limit2(float limit2) override;

    std::shared_ptr<Vec3> setLength(float len) override;

    std::shared_ptr<Vec3> setLength2(float len2) override;

    std::shared_ptr<Vec3> clamp(float min, float max) override;

    bool epsilonEquals(std::shared_ptr<Vec3> other, float epsilon) override;

    bool equals(const std::shared_ptr<Vec3>& obj);

    [[nodiscard]] bool epsilonEquals(float x1, float y1, float z1, float epsilon) const;

    bool epsilonEquals(const std::shared_ptr<Vec3>& other);

    [[nodiscard]] bool epislonEquals(float x1, float y1, float z1) const;

    std::shared_ptr<Vec3> setZero() override;
};

template<>
 struct std::hash<Vec3> {
     std::size_t operator()(const Vec3& vec) const noexcept;
 };

std::size_t std::hash<Vec3>::operator()(const Vec3 &vec) const noexcept {
    std::size_t prime = 31;
    std::size_t result = 1;
    result = prime * result + std::bit_cast<std::uint32_t>(vec.x);
    result = prime * result + std::bit_cast<std::uint32_t>(vec.y);
    result = prime * result + std::bit_cast<std::uint32_t>(vec.z);
    return result;
}

#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_VEC3_H
