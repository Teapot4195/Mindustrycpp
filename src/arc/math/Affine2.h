//
// Created by teapot on 24/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_AFFINE2_H
#define MINDUSTRY_SRC_ARC_MATH_AFFINE2_H

#include <memory>

// Forward decl to resolve circular reference;
class Mat;
class Vec2;

class Affine2 {
public:
    float m00 = 1, m01 = 0, m02 = 0;
    float m10 = 0, m11 = 1, m12 = 0;

    Affine2();

    explicit Affine2(const std::shared_ptr<Affine2>& other);

    Affine2(const Affine2&& other) noexcept;

    std::shared_ptr<Affine2> idt();

    std::shared_ptr<Affine2> set(const std::shared_ptr<Affine2>& other);

    std::shared_ptr<Affine2> set(const std::shared_ptr<Mat>& other);

    std::shared_ptr<Affine2> set(const Affine2&& other) noexcept;

    std::shared_ptr<Affine2> setToTranslation(float x, float y);

    std::shared_ptr<Affine2> setToTranslation(const std::shared_ptr<Vec2>& trn);

    std::shared_ptr<Affine2> setToScaling(float scaleX, float scaleY);

    std::shared_ptr<Affine2> setToScaling(const std::shared_ptr<Vec2>& scale);

    std::shared_ptr<Affine2> setToRotation(float degrees);

    std::shared_ptr<Affine2> setToRotationRad(float radians);

    std::shared_ptr<Affine2> setToRotation(float sin, float cos);

    std::shared_ptr<Affine2> setToShearing(float shearX, float shearY);

    std::shared_ptr<Affine2> setToShearing(const std::shared_ptr<Vec2>& shear);

    std::shared_ptr<Affine2> setToTrnRotScale(float x, float y, float degrees, float scaleX, float scaleY);

    std::shared_ptr<Affine2> setToTrnRotScale(std::shared_ptr<Vec2> trn, float degrees, std::shared_ptr<Vec2> scale);

    std::shared_ptr<Affine2> setToTrnRotRadScale(float x, float y, float radians, float scaleX, float scaleY);

    std::shared_ptr<Affine2> setToTrnRotRadScale(const std::shared_ptr<Vec2>& trn, float degrees, const std::shared_ptr<Vec2>& scale);

    std::shared_ptr<Affine2> setToTrnScl(float x, float y, float scaleX, float scaleY);

    std::shared_ptr<Affine2> setToTrnScl(const std::shared_ptr<Vec2>& trn, const std::shared_ptr<Vec2>& scale);

    std::shared_ptr<Affine2> setToProduct(const std::shared_ptr<Affine2>& l, const std::shared_ptr<Affine2>& r);

    std::shared_ptr<Affine2> inv();

    std::shared_ptr<Affine2> mul(const std::shared_ptr<Affine2>& other);

    std::shared_ptr<Affine2> preMul(const std::shared_ptr<Affine2>& other);

    std::shared_ptr<Affine2> translate(float x, float y);

    std::shared_ptr<Affine2> translate(const std::shared_ptr<Vec2>& trn);

    std::shared_ptr<Affine2> preTranslate(float x, float y);

    std::shared_ptr<Affine2> preTranslate(const std::shared_ptr<Vec2>& trn);

    std::shared_ptr<Affine2> scale(float scaleX, float scaleY);

    std::shared_ptr<Affine2> scale(const std::shared_ptr<Vec2>& scale);

    std::shared_ptr<Affine2> preScale(float scaleX, float scaleY);

    std::shared_ptr<Affine2> preScale(const std::shared_ptr<Vec2>& scale);

    std::shared_ptr<Affine2> rotate(float degrees);

    std::shared_ptr<Affine2> rotateRad(float radians);

    std::shared_ptr<Affine2> preRotate(float degrees);

    std::shared_ptr<Affine2> preRotateRad(float radians);

    std::shared_ptr<Affine2> shear(float shearX, float shearY);

    std::shared_ptr<Affine2> shear(const std::shared_ptr<Vec2>& shear);

    std::shared_ptr<Affine2> preShear(float shearX, float shearY);

    std::shared_ptr<Affine2> preShear(const std::shared_ptr<Vec2>& shear);

    [[nodiscard]] float det() const;

    [[nodiscard]] std::shared_ptr<Vec2> getTranslation(const std::shared_ptr<Vec2>& pos) const;

    [[nodiscard]] bool isTranslation() const;

    [[nodiscard]] bool isIdt() const;

    void applyTo(const std::shared_ptr<Vec2>& point) const;

    [[nodiscard]] std::string toString() const;
};


#endif //MINDUSTRY_SRC_ARC_MATH_AFFINE2_H
