//
// Created by teapot on 23/11/23.
//

#include "Vec3.h"
#include "math/Mathf.h"
#include "util/Exception.h"

#include <cmath>
#include <utility>

#include <math/Mat.h>
#include <math/geom/Vec2.h>
#include <math/Interp.h>

const std::shared_ptr<Vec3> Vec3::X = std::make_shared<Vec3>(1, 0, 0);
const std::shared_ptr<Vec3> Vec3::Y = std::make_shared<Vec3>(0, 1, 0);
const std::shared_ptr<Vec3> Vec3::Z = std::make_shared<Vec3>(0, 0, 1);
const std::shared_ptr<Vec3> Vec3::Zero = std::make_shared<Vec3>(0, 0, 0);

std::shared_ptr<Mat> Vec3::tmpMat = std::make_shared<Mat>();

Vec3::Vec3() = default;

Vec3::Vec3(float x, float y, float z) {
    set(x, y, z);
}

Vec3::Vec3(const std::shared_ptr<Vec3> &vector) {
    set(vector);
}

Vec3::Vec3(std::vector<float> values) {
    set(values[0], values[1], values[2]);
}

Vec3::Vec3(const std::shared_ptr<Vec2> &vector, float z) {
    set(vector->x, vector->y, z);
}

float Vec3::len(float x, float y, float z) {
    return (float)std::sqrt(x * x + y * y + z * z);
}

float Vec3::len2(float x, float y, float z) {
    return x * x + y * y + z * z;
}

float Vec3::dst(float x1, float y1, float z1, float x2, float y2, float z2) {
    float a = x2 - x1;
    float b = y2 - y1;
    float c = z2 - z1;
    return std::sqrt(a * a + b * b + c * c);
}

float Vec3::dst2(float x1, float y1, float z1, float x2, float y2, float z2) {
    float a = x2 - x1;
    float b = y2 - y1;
    float c = z2 - z1;
    return a * a + b * b + c * c;
}

float Vec3::dot(float x1, float y1, float z1, float x2, float y2, float z2) {
    return x1 * x2 + y1 * y2 + z1 * z2;
}

std::shared_ptr<Vec3> Vec3::set(float x1, float y1, float z1) {
    this->x = x1;
    this->y = y1;
    this->z = z1;

    return shared_from_this();
}

std::shared_ptr<Vec3> Vec3::div(std::shared_ptr<Vec3> other) {
    x /= other->x;
    y /= other->y;
    z /= other->z;

    return shared_from_this();
}

std::shared_ptr<Vec3> Vec3::set(std::shared_ptr<Vec3> v) {
    return set(v->x, v->y, v->z);
}

std::shared_ptr<Vec3> Vec3::set(std::vector<float> values) {
    return set(values[0], values[1], values[2]);
}

std::shared_ptr<Vec3> Vec3::set(std::vector<float> values, std::size_t offset) {
    return set(values[0 + offset], values[1 + offset], values[2 + offset]);
}

std::shared_ptr<Vec3> Vec3::set(const std::shared_ptr<Vec2> &vector, float z1) {
    set(vector->x, vector->y, z1);
}

std::shared_ptr<Vec3> Vec3::setFromSpherical(float azimuthalAngle, float polarAngle) {
    float cosPolar = Mathf::cos(polarAngle);
    float sinPolar = Mathf::sin(polarAngle);

    float cosAzim = Mathf::cos(azimuthalAngle);
    float sinAzim = Mathf::sin(azimuthalAngle);

    return set(cosAzim * sinPolar, sinAzim * sinPolar, cosPolar);
}

std::shared_ptr<Vec3> Vec3::setToRandomDirection() {
    return setToRandomDirection(Mathf::rand);
}

std::shared_ptr<Vec3> Vec3::setToRandomDirection(Rand rand) {
    float u = rand.random(1.f);
    float v = rand.random(1.f);

    float theta = Mathf::PI2 * u; // azimuthal angle
    auto phi = (float)std::acos(2.f * v - 1.f); // polar angle

    return setFromSpherical(theta, phi);
}

std::shared_ptr<Vec3> Vec3::cpy() {
    return std::make_shared<Vec3>(shared_from_this());
}

std::shared_ptr<Vec3> Vec3::add(std::shared_ptr<Vec3> v) {
    return add(v->x, v->y, v->z);
}

std::shared_ptr<Vec3> Vec3::cpy(const std::shared_ptr<Vec3>& dest) {
    return dest->set(shared_from_this());
}

std::shared_ptr<Vec3> Vec3::add(const std::shared_ptr<Vec3>& vector, float scale) {
    return add(vector->x * scale, vector->y * scale, vector->z * scale);
}

std::shared_ptr<Vec3> Vec3::sub(const std::shared_ptr<Vec3>& vector, float scale) {
    return sub(vector->x * scale, vector->y * scale, vector->z * scale);
}

std::shared_ptr<Vec3> Vec3::add(float x1, float y1, float z1) {
    return set(x + x1, y + y1, z + z1);
}

std::shared_ptr<Vec3> Vec3::sub(std::shared_ptr<Vec3> v) {
    return sub(v->x, v->y, v->z);
}

std::shared_ptr<Vec3> Vec3::sub(float x1, float y1, float z1) {
    return set(x - x1, y - y1, z - z1);
}

std::shared_ptr<Vec3> Vec3::sub(float value) {
    return set(x - value, y - value, z - value);
}

std::shared_ptr<Vec3> Vec3::scl(float scalar) {
    return set(x * scalar, y * scalar, z * scalar);
}

std::shared_ptr<Vec3> Vec3::scl(std::shared_ptr<Vec3> v) {
    return set(x * v->x, y * v->y, z * v->z);
}

std::shared_ptr<Vec3> Vec3::scl(float vx, float vy, float vz) {
    return set(x * vx, y * vy, z * vz);
}

std::shared_ptr<Vec3> Vec3::mulAdd(std::shared_ptr<Vec3> v, float scalar) {
    x += v->x * scalar;
    y += v->y * scalar;
    z += v->z * scalar;
    return shared_from_this();
}

std::shared_ptr<Vec3> Vec3::mulAdd(std::shared_ptr<Vec3> v, std::shared_ptr<Vec3> mulVec) {
    x += v->x * mulVec->x;
    y += v->y * mulVec->y;
    z += v->z * mulVec->z;
    return shared_from_this();
}

float Vec3::len() {
    return std::sqrt(x * x + y * y + z * z);
}

float Vec3::len2() {
    return x * x + y * y + z * z;
}

bool Vec3::idt(const std::shared_ptr<Vec3>& vector) const {
    return x == vector->x && y == vector->y && z == vector->z;
}

float Vec3::dst(std::shared_ptr<Vec3> v) {
    float a = v->x - x;
    float b = v->y - y;
    float c = v->z - z;
    return std::sqrt(a * a + b * b + c * c);
}

float Vec3::dst2(std::shared_ptr<Vec3> v) {
    float a = v->x - x;
    float b = v->y - y;
    float c = v->z - z;
    return a * a + b * b + c * c;
}

float Vec3::dst(float x1, float y1, float z1) const {
    float a = x1 - x;
    float b = y1 - y;
    float c = z1 - z;
    return std::sqrt(a * a + b * b + c * c);
}

float Vec3::dst2(float x1, float y1, float z1) const {
    float a = x1 - x;
    float b = y1 - y;
    float c = z1 - z;
    return a * a + b * b + c * c;
}

bool Vec3::within(std::shared_ptr<Vec3> v, float dst) {
    return dst2(std::move(v)) < dst * dst;
}

std::shared_ptr<Vec3> Vec3::nor() {
    float l2 = len2();
    if (l2 == 0.f || l2 == 1.f)
        return shared_from_this();
    return scl(1.f / (float)std::sqrt(l2));
}

float Vec3::dot(std::shared_ptr<Vec3> v) {
    return x * v->x + y * v->y + z * v->z;
}

float Vec3::angleRad(const std::shared_ptr<Vec3>& vector) {
    float l = len();
    float l2 = vector->len();
    return std::acos(dot(x / l, y / l, z / l, vector->x / l2, vector->y / l2, vector->z / l2));
}

float Vec3::angle(const std::shared_ptr<Vec3>& vector) {
    return angleRad(vector) * Mathf::radDeg;
}

float Vec3::dot(float x1, float y1, float z1) const {
    return x * x1 + y * y1 + z * z1;
}

std::shared_ptr<Vec3> Vec3::crs(const std::shared_ptr<Vec3>& vector) {
    return set(y * vector->z - z * vector->y, z * vector->x - x * vector->z, x * vector->y - y * vector->x);
}

std::shared_ptr<Vec3> Vec3::crs(float x1, float y1, float z1) {
    return set(y * z1 - z * y1, z * x1 - x * z1, x * y1 - y * x1);
}

std::shared_ptr<Vec3> Vec3::mul4x3(std::vector<float> matrix) {
    return set(x * matrix[0] + y * matrix[3] + z * matrix[6] + matrix[9],
               x * matrix[1] + y * matrix[4] + z * matrix[7] + matrix[10],
               x * matrix[2] + y * matrix[5] + z * matrix[8] + matrix[11]);
}

std::shared_ptr<Vec3> Vec3::mul(const std::shared_ptr<Mat>& matrix) {
    auto l_mat = matrix->val;
    return set(x * l_mat[Mat::M00] + y * l_mat[Mat::M01] + z * l_mat[Mat::M02],
               x * l_mat[Mat::M10] + y * l_mat[Mat::M11] + z * l_mat[Mat::M12],
               x * l_mat[Mat::M20] + y * l_mat[Mat::M21] + z * l_mat[Mat::M22]);
}

std::shared_ptr<Vec3> Vec3::traMul(const std::shared_ptr<Mat>& matrix) {
    auto l_mat = matrix->val;
    return set(x * l_mat[Mat::M00] + y * l_mat[Mat::M10] + z * l_mat[Mat::M20],
               x * l_mat[Mat::M01] + y * l_mat[Mat::M11] + z * l_mat[Mat::M21],
               x * l_mat[Mat::M02] + y * l_mat[Mat::M12] + z * l_mat[Mat::M22]);
}

std::shared_ptr<Vec3> Vec3::rotate(const std::shared_ptr<Vec3>& axis, float degrees) {
    tmpMat->setToRotation(axis, degrees);
    return mul(tmpMat);
}

bool Vec3::isUnit() {
    return isUnit(0.000000001f);
}

bool Vec3::isUnit(float margin) {
    return std::abs(len2() - 1.f) < margin;
}

bool Vec3::isZero() {
    return x == 0 && y == 0 && z == 0;
}

bool Vec3::isZero(float margin) {
    return len2() < margin;
}

bool Vec3::isOnLine(std::shared_ptr<Vec3> other) {
    return len2(y * other->z - z * other->y,
                z * other->x - x * other->z,
                x * other->y - y * other->x
    ) <= Mathf::FLOAT_ROUNDING_ERROR;
}

bool Vec3::isOnLine(std::shared_ptr<Vec3> other, float epsilon) {
    return len2(y * other->z - z * other->y,
                z * other->x - x * other->z,
                x * other->y - y * other->x
    ) <= epsilon;
}

bool Vec3::isCollinear(std::shared_ptr<Vec3> other) {
    return isOnLine(other) && hasSameDirection(other);
}

bool Vec3::isCollinear(std::shared_ptr<Vec3> other, float epsilon) {
    return isOnLine(other, epsilon) && hasSameDirection(other);
}

bool Vec3::isCollinearOpposite(std::shared_ptr<Vec3> other, float epsilon) {
    return isOnLine(other, epsilon) && hasOppositeDirection(other);
}

bool Vec3::isCollinearOpposite(std::shared_ptr<Vec3> other) {
    return isOnLine(other) && hasOppositeDirection(other);
}

bool Vec3::isPerpendicular(std::shared_ptr<Vec3> other) {
    return Mathf::zero(dot(other));
}

bool Vec3::isPerpendicular(std::shared_ptr<Vec3> other, float epsilon) {
    return Mathf::zero(dot(other), epsilon);
}

bool Vec3::hasSameDirection(std::shared_ptr<Vec3> other) {
    return dot(other) > 0;
}

bool Vec3::hasOppositeDirection(std::shared_ptr<Vec3> other) {
    return dot(other) < 0;
}

std::shared_ptr<Vec3> Vec3::lerp(std::shared_ptr<Vec3> target, float alpha) {
    x += alpha * (target->x - x);
    y += alpha * (target->y - y);
    z += alpha * (target->z - z);
    return shared_from_this();
}

std::shared_ptr<Vec3>
Vec3::interpolate(std::shared_ptr<Vec3> target, float alpha, std::shared_ptr<Interp> interpolator) {
    return lerp(target, interpolator->apply(0.f, 1.f, alpha));
}

std::shared_ptr<Vec3> Vec3::slerp(const std::shared_ptr<Vec3>& target, float alpha) {
    float d = dot(target);
    if (d > 0.9995 || d < -0.9995)
        return lerp(target, alpha);

    auto theta0 = (float)std::acos(d);
    float theta = theta0 * alpha;

    auto st = (float)std::sin(theta);
    float tx = target->x - x * d;
    float ty = target->y - y * d;
    float tz = target->z - z * d;
    float l2 = tx * tx + ty * ty + tz * tz;
    float dl = st * ((l2 < 0.0001f) ? 1.f : 1.f / (float) std::sqrt(l2));

    return scl((float)std::cos(theta))->add(tx * dl, ty * dl, tz * dl)->nor();
}

std::string Vec3::toString() const {
    return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
}

std::shared_ptr<Vec3> Vec3::fromString(std::string v) {
    std::size_t s0 = v.find_first_of(',');
    std::size_t s1 = v.find_first_of(',', s0 + 1);

    if (s0 != std::string::npos && s1 != std::string::npos && v[0] == '(' && v[v.length() - 1] == ')') {
        try {
            float x1 = std::stof(v.substr(1, s0));
            float y1 = std::stof(v.substr(s0 + 1, s1));
            float z1 = std::stof(v.substr(s1 + 1, v.length() - 1));
            return set(x1, y1, z1);
        } catch (const std::invalid_argument& ex) {
            // Throw a ArcRuntimeException
        }
    }
    throw ArcRuntimeException("Malformed Vec3:" + v);
}

std::shared_ptr<Vec3> Vec3::limit(float limit) {
    return limit2(limit * limit);
}

std::shared_ptr<Vec3> Vec3::limit2(float limit2) {
    float l2 = len2();
    if (l2 > limit2) {
        scl((float)std::sqrt(limit2 / l2));
    }
    return shared_from_this();
}

std::shared_ptr<Vec3> Vec3::setLength(float len) {
    return setLength2(len * len);
}

std::shared_ptr<Vec3> Vec3::setLength2(float l2) {
    float oldLen2 = len2();
    return (oldLen2 == 0 || oldLen2 == l2) ? shared_from_this() : scl((float)std::sqrt(l2 / oldLen2));
}

std::shared_ptr<Vec3> Vec3::clamp(float min, float max) {
    float l2 = len2();
    if (l2 == 0.f)
        return shared_from_this();
    float max2 = max * max;
    if (l2 > max2)
        return scl((float)std::sqrt(max2 / l2));
    float min2 = min * min;
    if (l2 < min2)
        return scl((float)std::sqrt(min2 / l2));
    return shared_from_this();
}

bool Vec3::epsilonEquals(std::shared_ptr<Vec3> other, float epsilon) {
    if (!other)
        return false;
    if (std::abs(other->x - x) > epsilon)
        return false;
    if (std::abs(other->y - y) > epsilon)
        return false;
    return std::abs(other->z - z) <= epsilon;
}

bool Vec3::equals(const std::shared_ptr<Vec3>& obj) {
    if (obj.get() == this)
        return true;
    if (!obj)
        return false;
    return std::bit_cast<std::uint32_t>(x) == std::bit_cast<std::uint32_t>(obj->x) &&
           std::bit_cast<std::uint32_t>(y) == std::bit_cast<std::uint32_t>(obj->y) &&
           std::bit_cast<std::uint32_t>(z) == std::bit_cast<std::uint32_t>(obj->z);
}

bool Vec3::epsilonEquals(float x1, float y1, float z1, float epsilon) const {
    if (std::abs(x1 - x) > epsilon)
        return false;
    if (std::abs(y1 - y) > epsilon)
        return false;
    return std::abs(z1 - z) <= epsilon;
}

bool Vec3::epsilonEquals(const std::shared_ptr<Vec3> &other) {
    return epsilonEquals(other, Mathf::FLOAT_ROUNDING_ERROR);
}

bool Vec3::epislonEquals(float x1, float y1, float z1) const {
    return epsilonEquals(x1, y1, z1, Mathf::FLOAT_ROUNDING_ERROR);
}

std::shared_ptr<Vec3> Vec3::setZero() {
    x = 0; y = 0; z = 0;
    return shared_from_this();
}
