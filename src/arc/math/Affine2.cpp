//
// Created by teapot on 24/11/23.
//

#include "Affine2.h"

#include <math/geom/Vec2.h>
#include <util/ArcRuntimeException.h>
#include "Mathf.h"
#include "Mat.h"

Affine2::Affine2() = default;


Affine2::Affine2(const std::shared_ptr<Affine2>& other) {
    set(other);
}

Affine2::Affine2(const Affine2 &&other) noexcept {
    set(std::move(other));
}

std::shared_ptr<Affine2> Affine2::idt() {
    m00 = 1;
    m01 = 0;
    m02 = 0;
    m10 = 0;
    m11 = 1;
    m12 = 0;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::set(const Affine2 &&other) noexcept {
    m00 = other.m00;
    m01 = other.m01;
    m02 = other.m02;
    m10 = other.m10;
    m11 = other.m11;
    m12 = other.m12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::set(const std::shared_ptr<Affine2>& other) {
    m00 = other->m00;
    m01 = other->m01;
    m02 = other->m02;
    m10 = other->m10;
    m11 = other->m11;
    m12 = other->m12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::set(const std::shared_ptr<Mat> &matrix) {
    float* other = matrix->val;

    m00 = other[Mat::M00];
    m01 = other[Mat::M01];
    m02 = other[Mat::M02];
    m10 = other[Mat::M10];
    m11 = other[Mat::M11];
    m12 = other[Mat::M12];
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::setToTranslation(float x, float y) {
    m00 = 1;
    m01 = 0;
    m02 = x;
    m10 = 0;
    m11 = 1;
    m12 = y;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::setToTranslation(const std::shared_ptr<Vec2> &trn) {
    return setToTranslation(trn->x, trn->y);
}

std::shared_ptr<Affine2> Affine2::setToScaling(float scaleX, float scaleY) {
    m00 = scaleX;
    m01 = 0;
    m02 = 0;
    m10 = 0;
    m11 = scaleY;
    m12 = 0;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::setToScaling(const std::shared_ptr<Vec2> &scale) {
    return setToScaling(scale->x, scale->y);
}

std::shared_ptr<Affine2> Affine2::setToRotation(float degrees) {
    float cos = Mathf::cosDeg(degrees);
    float sin = Mathf::sinDeg(degrees);

    m00 = cos;
    m01 = -sin;
    m02 = 0;
    m10 = sin;
    m11 = cos;
    m12 = 0;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::setToRotationRad(float radians) {
    float cos = Mathf::cos(radians);
    float sin = Mathf::sin(radians);

    m00 = cos;
    m01 = -sin;
    m02 = 0;
    m10 = sin;
    m11 = cos;
    m12 = 0;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::setToRotation(float sin, float cos) {
    m00 = cos;
    m01 = -sin;
    m02 = 0;
    m10 = sin;
    m11 = cos;
    m12 = 0;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::setToShearing(float shearX, float shearY) {
    m00 = 1;
    m01 = shearX;
    m02 = 0;
    m10 = shearY;
    m11 = 1;
    m12 = 0;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::setToShearing(const std::shared_ptr<Vec2>& shear) {
    return setToShearing(shear->x, shear->y);
}

std::shared_ptr<Affine2> Affine2::setToTrnRotScale(float x, float y, float degrees, float scaleX, float scaleY) {
    m02 = x;
    m12 = y;

    if(degrees == 0){
        m00 = scaleX;
        m01 = 0;
        m10 = 0;
        m11 = scaleY;
    }else{
        float sin = Mathf::sinDeg(degrees);
        float cos = Mathf::cosDeg(degrees);

        m00 = cos * scaleX;
        m01 = -sin * scaleY;
        m10 = sin * scaleX;
        m11 = cos * scaleY;
    }
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2>
Affine2::setToTrnRotScale(std::shared_ptr<Vec2> trn, float degrees, std::shared_ptr<Vec2> scale) {
    return setToTrnRotScale(trn->x, trn->y, degrees, scale->x, scale->y);
}

std::shared_ptr<Affine2> Affine2::setToTrnRotRadScale(float x, float y, float radians, float scaleX, float scaleY) {
    m02 = x;
    m12 = y;

    if(radians == 0){
        m00 = scaleX;
        m01 = 0;
        m10 = 0;
        m11 = scaleY;
    }else{
        float sin = Mathf::sin(radians);
        float cos = Mathf::cos(radians);

        m00 = cos * scaleX;
        m01 = -sin * scaleY;
        m10 = sin * scaleX;
        m11 = cos * scaleY;
    }
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2>
Affine2::setToTrnRotRadScale(const std::shared_ptr<Vec2>& trn, float degrees, const std::shared_ptr<Vec2>& scale) {
    return setToTrnRotRadScale(trn->x, trn->y, degrees, scale->x, scale->y);
}

std::shared_ptr<Affine2> Affine2::setToTrnScl(float x, float y, float scaleX, float scaleY) {
    m00 = scaleX;
    m01 = 0;
    m02 = x;
    m10 = 0;
    m11 = scaleY;
    m12 = y;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::setToTrnScl(const std::shared_ptr<Vec2>& trn, const std::shared_ptr<Vec2>& scale) {
    return setToTrnScl(trn->x, trn->y, scale->x, scale->y);
}

std::shared_ptr<Affine2> Affine2::setToProduct(const std::shared_ptr<Affine2> &l, const std::shared_ptr<Affine2> &r) {
    m00 = l->m00 * r->m00 + l->m01 * r->m10;
    m01 = l->m00 * r->m01 + l->m01 * r->m11;
    m02 = l->m00 * r->m02 + l->m01 * r->m12 + l->m02;
    m10 = l->m10 * r->m00 + l->m11 * r->m10;
    m11 = l->m10 * r->m01 + l->m11 * r->m11;
    m12 = l->m10 * r->m02 + l->m11 * r->m12 + l->m12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::inv() {
    float d = det();
    if(d == 0)
        throw ArcRuntimeException("Can't invert a singular affine matrix");

    float invDet = 1.0f / d;

    float tmp00 = m11;
    float tmp01 = -m01;
    float tmp02 = m01 * m12 - m11 * m02;
    float tmp10 = -m10;
    float tmp11 = m00;
    float tmp12 = m10 * m02 - m00 * m12;

    m00 = invDet * tmp00;
    m01 = invDet * tmp01;
    m02 = invDet * tmp02;
    m10 = invDet * tmp10;
    m11 = invDet * tmp11;
    m12 = invDet * tmp12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::mul(const std::shared_ptr<Affine2> &other) {
    float tmp00 = m00 * other->m00 + m01 * other->m10;
    float tmp01 = m00 * other->m01 + m01 * other->m11;
    float tmp02 = m00 * other->m02 + m01 * other->m12 + m02;
    float tmp10 = m10 * other->m00 + m11 * other->m10;
    float tmp11 = m10 * other->m01 + m11 * other->m11;
    float tmp12 = m10 * other->m02 + m11 * other->m12 + m12;

    m00 = tmp00;
    m01 = tmp01;
    m02 = tmp02;
    m10 = tmp10;
    m11 = tmp11;
    m12 = tmp12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::preMul(const std::shared_ptr<Affine2> &other) {
    float tmp00 = other->m00 * m00 + other->m01 * m10;
    float tmp01 = other->m00 * m01 + other->m01 * m11;
    float tmp02 = other->m00 * m02 + other->m01 * m12 + other->m02;
    float tmp10 = other->m10 * m00 + other->m11 * m10;
    float tmp11 = other->m10 * m01 + other->m11 * m11;
    float tmp12 = other->m10 * m02 + other->m11 * m12 + other->m12;

    m00 = tmp00;
    m01 = tmp01;
    m02 = tmp02;
    m10 = tmp10;
    m11 = tmp11;
    m12 = tmp12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::translate(float x, float y) {
    m02 += m00 * x + m01 * y;
    m12 += m10 * x + m11 * y;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::translate(const std::shared_ptr<Vec2> &trn) {
    return translate(trn->x, trn->y);
}

std::shared_ptr<Affine2> Affine2::preTranslate(float x, float y) {
    m02 += x;
    m12 += y;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::preTranslate(const std::shared_ptr<Vec2> &trn) {
    return preTranslate(trn->x, trn->y);
}

std::shared_ptr<Affine2> Affine2::scale(float scaleX, float scaleY) {
    m00 *= scaleX;
    m01 *= scaleY;
    m10 *= scaleX;
    m11 *= scaleY;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::scale(const std::shared_ptr<Vec2>& scale) {
    return this->scale(scale->x, scale->y);
}

std::shared_ptr<Affine2> Affine2::preScale(float scaleX, float scaleY) {
    m00 *= scaleX;
    m01 *= scaleX;
    m02 *= scaleX;
    m10 *= scaleY;
    m11 *= scaleY;
    m12 *= scaleY;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::preScale(const std::shared_ptr<Vec2> &scale) {
    return preScale(scale->x, scale->y);
}

std::shared_ptr<Affine2> Affine2::rotate(float degrees) {
    if(degrees == 0)
        return std::shared_ptr<Affine2>(this);

    float cos = Mathf::cosDeg(degrees);
    float sin = Mathf::sinDeg(degrees);

    float tmp00 = m00 * cos + m01 * sin;
    float tmp01 = m00 * -sin + m01 * cos;
    float tmp10 = m10 * cos + m11 * sin;
    float tmp11 = m10 * -sin + m11 * cos;

    m00 = tmp00;
    m01 = tmp01;
    m10 = tmp10;
    m11 = tmp11;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::rotateRad(float radians) {
    if(radians == 0)
        return std::shared_ptr<Affine2>(this);

    float cos = Mathf::cos(radians);
    float sin = Mathf::sin(radians);

    float tmp00 = m00 * cos + m01 * sin;
    float tmp01 = m00 * -sin + m01 * cos;
    float tmp10 = m10 * cos + m11 * sin;
    float tmp11 = m10 * -sin + m11 * cos;

    m00 = tmp00;
    m01 = tmp01;
    m10 = tmp10;
    m11 = tmp11;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::preRotate(float degrees) {
    if(degrees == 0)
        return std::shared_ptr<Affine2>(this);

    float cos = Mathf::cosDeg(degrees);
    float sin = Mathf::sinDeg(degrees);

    float tmp00 = cos * m00 - sin * m10;
    float tmp01 = cos * m01 - sin * m11;
    float tmp02 = cos * m02 - sin * m12;
    float tmp10 = sin * m00 + cos * m10;
    float tmp11 = sin * m01 + cos * m11;
    float tmp12 = sin * m02 + cos * m12;

    m00 = tmp00;
    m01 = tmp01;
    m02 = tmp02;
    m10 = tmp10;
    m11 = tmp11;
    m12 = tmp12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::preRotateRad(float radians) {
    if(radians == 0)
        return std::shared_ptr<Affine2>(this);

    float cos = Mathf::cos(radians);
    float sin = Mathf::sin(radians);

    float tmp00 = cos * m00 - sin * m10;
    float tmp01 = cos * m01 - sin * m11;
    float tmp02 = cos * m02 - sin * m12;
    float tmp10 = sin * m00 + cos * m10;
    float tmp11 = sin * m01 + cos * m11;
    float tmp12 = sin * m02 + cos * m12;

    m00 = tmp00;
    m01 = tmp01;
    m02 = tmp02;
    m10 = tmp10;
    m11 = tmp11;
    m12 = tmp12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::shear(float shearX, float shearY) {
    float tmp0 = m00 + shearY * m01;
    float tmp1 = m01 + shearX * m00;
    m00 = tmp0;
    m01 = tmp1;

    tmp0 = m10 + shearY * m11;
    tmp1 = m11 + shearX * m10;
    m10 = tmp0;
    m11 = tmp1;
        return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::shear(const std::shared_ptr<Vec2> &shear) {
    return this->shear(shear->x, shear->y);
}

std::shared_ptr<Affine2> Affine2::preShear(float shearX, float shearY) {
    float tmp00 = m00 + shearX * m10;
    float tmp01 = m01 + shearX * m11;
    float tmp02 = m02 + shearX * m12;
    float tmp10 = m10 + shearY * m00;
    float tmp11 = m11 + shearY * m01;
    float tmp12 = m12 + shearY * m02;

    m00 = tmp00;
    m01 = tmp01;
    m02 = tmp02;
    m10 = tmp10;
    m11 = tmp11;
    m12 = tmp12;
    return std::shared_ptr<Affine2>(this);
}

std::shared_ptr<Affine2> Affine2::preShear(const std::shared_ptr<Vec2> &shear) {
    return this->preShear(shear->x, shear->y);
}

float Affine2::det() const {
    return m00 * m11 - m01 * m10;
}

std::shared_ptr<Vec2> Affine2::getTranslation(const std::shared_ptr<Vec2> &pos) const {
    pos->x = m02;
    pos->y = m12;
    return pos;
}

bool Affine2::isTranslation() const {
    return (m00 == 1 && m11 == 1 && m01 == 0 && m10 == 0);
}

bool Affine2::isIdt() const {
    return (m00 == 1 && m02 == 0 && m12 == 0 && m11 == 1 && m01 == 0 && m10 == 0);
}

void Affine2::applyTo(const std::shared_ptr<Vec2>& point) const {
    float x = point->x;
    float y = point->y;
    point->x = m00 * x + m01 * y + m02;
    point->y = m10 * x + m11 * y + m12;
}

std::string Affine2::toString() const {
    return "[" + std::to_string(m00) + "|" + std::to_string(m01) + "|" + std::to_string(m02) + "]\n"
           "[" + std::to_string(m10) + "|" + std::to_string(m11) + "|" + std::to_string(m12) + "]\n"
           "[0.0|0.0|0.1]";
}
