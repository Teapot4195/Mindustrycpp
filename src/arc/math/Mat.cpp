//
// Created by teapot on 23/11/23.
//

#include "Mat.h"

#include <cmath>
#include <cstring>

#include <math/Mathf.h>
#include <math/Affine2.h>
#include <math/geom/Vec3.h>
#include <math/geom/Vec2.h>
#include <util/ArcRuntimeException.h>
#include <Utilities.h>

Mat::Mat() {
    idt();
}

Mat::Mat(const std::shared_ptr<Mat>& matrix) {
    set(matrix);
}

Mat::Mat(Mat *matrix) : Mat(std::shared_ptr<Mat>(matrix)) {}

Mat::Mat(float *values) {
    set(std::vector<float>(values, values + 9));
}

void Mat::mul(float *mata, const float *matb) {
    float v00 = mata[M00] * matb[M00] + mata[M01] * matb[M10] + mata[M02] * matb[M20];
    float v01 = mata[M00] * matb[M01] + mata[M01] * matb[M11] + mata[M02] * matb[M21];
    float v02 = mata[M00] * matb[M02] + mata[M01] * matb[M12] + mata[M02] * matb[M22];

    float v10 = mata[M10] * matb[M00] + mata[M11] * matb[M10] + mata[M12] * matb[M20];
    float v11 = mata[M10] * matb[M01] + mata[M11] * matb[M11] + mata[M12] * matb[M21];
    float v12 = mata[M10] * matb[M02] + mata[M11] * matb[M12] + mata[M12] * matb[M22];

    float v20 = mata[M20] * matb[M00] + mata[M21] * matb[M10] + mata[M22] * matb[M20];
    float v21 = mata[M20] * matb[M01] + mata[M21] * matb[M11] + mata[M22] * matb[M21];
    float v22 = mata[M20] * matb[M02] + mata[M21] * matb[M12] + mata[M22] * matb[M22];

    mata[M00] = v00;
    mata[M10] = v10;
    mata[M20] = v20;
    mata[M01] = v01;
    mata[M11] = v11;
    mata[M21] = v21;
    mata[M02] = v02;
    mata[M12] = v12;
    mata[M22] = v22;
}

std::shared_ptr<Mat> Mat::setOrtho(float x, float y, float width, float height) {
    const float right = x + width , top = y + height;

    float x_orth = 2 / (right - x);
    float y_orth = 2 / (top - y);

    float tx = -(right + x) / (right - x);
    float ty = -(top + y) / (top - y);

    val[M00] = x_orth;
    val[M11] = y_orth;

    val[M02] = tx;
    val[M12] = ty;
    val[M22] = 1.f;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::idt() {
    val[M00] = 1;
    val[M10] = 0;
    val[M20] = 0;
    val[M01] = 0;
    val[M11] = 1;
    val[M21] = 0;
    val[M02] = 0;
    val[M12] = 0;
    val[M22] = 1;
    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::mul(const std::shared_ptr<Mat>& m) {
    float v00 = val[M00] * m->val[M00] + val[M01] * m->val[M10] + val[M02] * m->val[M20];
    float v01 = val[M00] * m->val[M01] + val[M01] * m->val[M11] + val[M02] * m->val[M21];
    float v02 = val[M00] * m->val[M02] + val[M01] * m->val[M12] + val[M02] * m->val[M22];

    float v10 = val[M10] * m->val[M00] + val[M11] * m->val[M10] + val[M12] * m->val[M20];
    float v11 = val[M10] * m->val[M01] + val[M11] * m->val[M11] + val[M12] * m->val[M21];
    float v12 = val[M10] * m->val[M02] + val[M11] * m->val[M12] + val[M12] * m->val[M22];

    float v20 = val[M20] * m->val[M00] + val[M21] * m->val[M10] + val[M22] * m->val[M20];
    float v21 = val[M20] * m->val[M01] + val[M21] * m->val[M11] + val[M22] * m->val[M21];
    float v22 = val[M20] * m->val[M02] + val[M21] * m->val[M12] + val[M22] * m->val[M22];

    val[M00] = v00;
    val[M10] = v10;
    val[M20] = v20;
    val[M01] = v01;
    val[M11] = v11;
    val[M21] = v21;
    val[M02] = v02;
    val[M12] = v12;
    val[M22] = v22;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::mulLeft(const std::shared_ptr<Mat>& m) {
    float v00 = m->val[M00] * val[M00] + m->val[M01] * val[M10] + m->val[M02] * val[M20];
    float v01 = m->val[M00] * val[M01] + m->val[M01] * val[M11] + m->val[M02] * val[M21];
    float v02 = m->val[M00] * val[M02] + m->val[M01] * val[M12] + m->val[M02] * val[M22];

    float v10 = m->val[M10] * val[M00] + m->val[M11] * val[M10] + m->val[M12] * val[M20];
    float v11 = m->val[M10] * val[M01] + m->val[M11] * val[M11] + m->val[M12] * val[M21];
    float v12 = m->val[M10] * val[M02] + m->val[M11] * val[M12] + m->val[M12] * val[M22];

    float v20 = m->val[M20] * val[M00] + m->val[M21] * val[M10] + m->val[M22] * val[M20];
    float v21 = m->val[M20] * val[M01] + m->val[M21] * val[M11] + m->val[M22] * val[M21];
    float v22 = m->val[M20] * val[M02] + m->val[M21] * val[M12] + m->val[M22] * val[M22];

    val[M00] = v00;
    val[M10] = v10;
    val[M20] = v20;
    val[M01] = v01;
    val[M11] = v11;
    val[M21] = v21;
    val[M02] = v02;
    val[M12] = v12;
    val[M22] = v22;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::setToRotation(float degrees) {
    return setToRotationRad(Mathf::degreesToRadians * degrees);
}

std::shared_ptr<Mat> Mat::setToRotationRad(float radians) {
    auto cos = (float)std::cos(radians);
    auto sin = (float)std::sin(radians);

    val[M00] = cos;
    val[M10] = sin;
    val[M20] = 0;

    val[M01] = -sin;
    val[M11] = cos;
    val[M21] = 0;

    val[M02] = 0;
    val[M12] = 0;
    val[M22] = 1;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::setToRotation(const std::shared_ptr<Vec3>& axis, float degrees) {
    return setToRotation(axis, Mathf::cosDeg(degrees), Mathf::sinDeg(degrees));
}

std::shared_ptr<Mat> Mat::setToRotation(const std::shared_ptr<Vec3>& axis, float cos, float sin) {
    float oc = 1.0f - cos;
    val[M00] = oc * axis->x * axis->x + cos;
    val[M10] = oc * axis->x * axis->y - axis->z * sin;
    val[M20] = oc * axis->z * axis->x + axis->y * sin;
    val[M01] = oc * axis->x * axis->y + axis->z * sin;
    val[M11] = oc * axis->y * axis->y + cos;
    val[M21] = oc * axis->y * axis->z - axis->x * sin;
    val[M02] = oc * axis->z * axis->x - axis->y * sin;
    val[M12] = oc * axis->y * axis->z + axis->x * sin;
    val[M22] = oc * axis->z * axis->z + cos;
    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::setToTranslation(float x, float y) {
    val[M00] = 1;
    val[M10] = 0;
    val[M20] = 0;

    val[M01] = 0;
    val[M11] = 1;
    val[M21] = 0;

    val[M02] = x;
    val[M12] = y;
    val[M22] = 1;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::setToTranslation(const std::shared_ptr<Vec2>& translation) {
    val[M00] = 1;
    val[M10] = 0;
    val[M20] = 0;

    val[M01] = 0;
    val[M11] = 1;
    val[M21] = 0;

    val[M02] = translation->x;
    val[M12] = translation->y;
    val[M22] = 1;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::setToScaling(float scaleX, float scaleY) {
    val[M00] = scaleX;
    val[M10] = 0;
    val[M20] = 0;
    val[M01] = 0;
    val[M11] = scaleY;
    val[M21] = 0;
    val[M02] = 0;
    val[M12] = 0;
    val[M22] = 1;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::setToScaling(const std::shared_ptr<Vec2>& scale) {
    val[M00] = scale->x;
    val[M10] = 0;
    val[M20] = 0;
    val[M01] = 0;
    val[M11] = scale->y;
    val[M21] = 0;
    val[M02] = 0;
    val[M12] = 0;
    val[M22] = 1;
    return std::shared_ptr<Mat>(this);
}

std::string Mat::toString() {
    using namespace std::string_literals;
    return "[" + std::to_string(val[M00]) + "|" + std::to_string(val[M01]) + "|" + std::to_string(val[M02]) + "]\n"
           "[" + std::to_string(val[M10]) + "|" + std::to_string(val[M11]) + "|" + std::to_string(val[M12]) + "]\n"
           "[" + std::to_string(val[M20]) + "|" + std::to_string(val[M21]) + "|" + std::to_string(val[M22]) + "]";
}

float Mat::det() {
    return val[M00] * val[M11] * val[M22] + val[M01] * val[M12] * val[M20] + val[M02] * val[M10] * val[M21] - val[M00]
         * val[M12] * val[M21] - val[M01] * val[M10] * val[M22] - val[M02] * val[M11] * val[M20];
}

std::shared_ptr<Mat> Mat::inv() {
    float d = det();
    if(d == 0)
        throw ArcRuntimeException("Can't invert a singular matrix");

    float inv_det = 1.0f / d;

    tmp[M00] = val[M11] * val[M22] - val[M21] * val[M12];
    tmp[M10] = val[M20] * val[M12] - val[M10] * val[M22];
    tmp[M20] = val[M10] * val[M21] - val[M20] * val[M11];
    tmp[M01] = val[M21] * val[M02] - val[M01] * val[M22];
    tmp[M11] = val[M00] * val[M22] - val[M20] * val[M02];
    tmp[M21] = val[M20] * val[M01] - val[M00] * val[M21];
    tmp[M02] = val[M01] * val[M12] - val[M11] * val[M02];
    tmp[M12] = val[M10] * val[M02] - val[M00] * val[M12];
    tmp[M22] = val[M00] * val[M11] - val[M10] * val[M01];

    val[M00] = inv_det * tmp[M00];
    val[M10] = inv_det * tmp[M10];
    val[M20] = inv_det * tmp[M20];
    val[M01] = inv_det * tmp[M01];
    val[M11] = inv_det * tmp[M11];
    val[M21] = inv_det * tmp[M21];
    val[M02] = inv_det * tmp[M02];
    val[M12] = inv_det * tmp[M12];
    val[M22] = inv_det * tmp[M22];

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::set(const std::shared_ptr<Mat>& mat) {
    std::memcpy(val, mat->val, 9 * sizeof(float));
    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::set(const std::shared_ptr<Affine2>& affine) {
    val[M00] = affine->m00;
    val[M10] = affine->m10;
    val[M20] = 0;
    val[M01] = affine->m01;
    val[M11] = affine->m11;
    val[M21] = 0;
    val[M02] = affine->m02;
    val[M12] = affine->m12;
    val[M22] = 1;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::set(std::vector<float> values) {
    std::memcpy(val, values.data(), 9 * sizeof(float));

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::trn(const std::shared_ptr<Vec2> &vector) {
    val[M02] += vector->x;
    val[M12] += vector->y;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::trn(float x, float y) {
    val[M02] += x;
    val[M12] += y;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::trn(const std::shared_ptr<Vec3> &vector) {
    val[M02] += vector->x;
    val[M12] += vector->y;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::translate(float x, float y) {
    tmp[M00] = 1;
    tmp[M10] = 0;
    tmp[M20] = 0;

    tmp[M01] = 0;
    tmp[M11] = 1;
    tmp[M21] = 0;

    tmp[M02] = x;
    tmp[M12] = y;
    tmp[M22] = 1;
    mul(val, tmp);

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::trnalste(const std::shared_ptr<Vec2> &translation) {
    tmp[M00] = 1;
    tmp[M10] = 0;
    tmp[M20] = 0;

    tmp[M01] = 0;
    tmp[M11] = 1;
    tmp[M21] = 0;

    tmp[M02] = translation->x;
    tmp[M12] = translation->y;
    tmp[M22] = 1;
    mul(val, tmp);

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::rotate(float degrees) {
    return rotateRad(Mathf::degreesToRadians * degrees);
}

std::shared_ptr<Mat> Mat::rotateRad(float radians) {
    if(radians == 0)
        return std::shared_ptr<Mat>(this);
    auto cos = (float)std::cos(radians);
    auto sin = (float)std::sin(radians);

    tmp[M00] = cos;
    tmp[M10] = sin;
    tmp[M20] = 0;

    tmp[M01] = -sin;
    tmp[M11] = cos;
    tmp[M21] = 0;

    tmp[M02] = 0;
    tmp[M12] = 0;
    tmp[M22] = 1;
    mul(val, tmp);

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::scale(float scaleX, float scaleY) {
    tmp[M00] = scaleX;
    tmp[M10] = 0;
    tmp[M20] = 0;
    tmp[M01] = 0;
    tmp[M11] = scaleY;
    tmp[M21] = 0;
    tmp[M02] = 0;
    tmp[M12] = 0;
    tmp[M22] = 1;
    mul(val, tmp);

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::scale(const std::shared_ptr<Vec2> &scale) {
    tmp[M00] = scale->x;
    tmp[M10] = 0;
    tmp[M20] = 0;
    tmp[M01] = 0;
    tmp[M11] = scale->y;
    tmp[M21] = 0;
    tmp[M02] = 0;
    tmp[M12] = 0;
    tmp[M22] = 1;
    mul(val, tmp);

    return std::shared_ptr<Mat>(this);
}

std::vector<float> Mat::getValues() const {
    return {val, val + 9};
}

std::shared_ptr<Vec2> Mat::getTranslation(const std::shared_ptr<Vec2> &position) {
    position->x = val[M02];
    position->y = val[M12];

    return position;
}

std::shared_ptr<Vec2> Mat::getScale(const std::shared_ptr<Vec2> &scale) {
    scale->x = (float)std::sqrt(val[M00] * val[M00] + val[M01] * val[M01]);
    scale->y = (float)std::sqrt(val[M10] * val[M10] + val[M11] * val[M11]);

    return scale;
}

float Mat::getRotation() {
    return Mathf::radiansToDegrees * (float)Mathf::atan2(val[M10], val[M00]);
}

float Mat::getRotationRad() {
    return Mathf::atan2(val[M10], val[M00]);
}

std::shared_ptr<Mat> Mat::scl(float scale) {
    val[M00] *= scale;
    val[M11] *= scale;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::scl(const std::shared_ptr<Vec2> &scale) {
    val[M00] *= scale->x;
    val[M11] *= scale->y;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::scl(const std::shared_ptr<Vec3> &scale) {
    val[M00] *= scale->x;
    val[M11] *= scale->y;

    return std::shared_ptr<Mat>(this);
}

std::shared_ptr<Mat> Mat::transpose() {
    float v01 = val[M10];
    float v02 = val[M20];
    float v10 = val[M01];
    float v12 = val[M21];
    float v20 = val[M02];
    float v21 = val[M12];
    val[M01] = v01;
    val[M02] = v02;
    val[M10] = v10;
    val[M12] = v12;
    val[M20] = v20;
    val[M21] = v21;

    return std::shared_ptr<Mat>(this);
}
