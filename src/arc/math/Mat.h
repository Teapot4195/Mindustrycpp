//
// Created by teapot on 23/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_MAT_H
#define MINDUSTRY_SRC_ARC_MATH_MAT_H

#include <memory>
#include <vector>

// Forward declaration to resolve circular dependency
class Vec3;
class Vec2;
class Affine2;

class Mat {
public:
    static const int M00 = 0;
    static const int M01 = 3;
    static const int M02 = 6;
    static const int M10 = 1;
    static const int M11 = 4;
    static const int M12 = 7;
    static const int M20 = 2;
    static const int M21 = 5;
    static const int M22 = 8;
    float val[9]{};
    float tmp[9]{};

    Mat();

    Mat(const std::shared_ptr<Mat>& matrix);

    Mat(Mat* matrix);

    Mat(float* values);

private:
    static void mul(float *mata, const float* matb);

public:
    std::shared_ptr<Mat> setOrtho(float x, float y, float width, float height);

    std::shared_ptr<Mat> idt();

    std::shared_ptr<Mat> mul(const std::shared_ptr<Mat>& m);

    std::shared_ptr<Mat> mulLeft(const std::shared_ptr<Mat>& m);

    std::shared_ptr<Mat> setToRotation(float degrees);

    std::shared_ptr<Mat> setToRotationRad(float radians);

    std::shared_ptr<Mat> setToRotation(const std::shared_ptr<Vec3>& axis, float degrees);

    std::shared_ptr<Mat> setToRotation(const std::shared_ptr<Vec3>& axis, float cos, float sin);

    std::shared_ptr<Mat> setToTranslation(float x, float y);

    std::shared_ptr<Mat> setToTranslation(const std::shared_ptr<Vec2>& translation);

    std::shared_ptr<Mat> setToScaling(float scaleX, float scaleY);

    std::shared_ptr<Mat> setToScaling(const std::shared_ptr<Vec2>& scale);

    std::string toString();

    float det();

    std::shared_ptr<Mat> inv();

    std::shared_ptr<Mat> set(const std::shared_ptr<Mat>& mat);

    std::shared_ptr<Mat> set(const std::shared_ptr<Affine2>& affine);

    std::shared_ptr<Mat> set(std::vector<float> values);

    std::shared_ptr<Mat> trn(const std::shared_ptr<Vec2>& vector);

    std::shared_ptr<Mat> trn(float x, float y);

    std::shared_ptr<Mat> trn(const std::shared_ptr<Vec3>& vector);

    std::shared_ptr<Mat> translate(float x, float y);

    std::shared_ptr<Mat> trnalste(const std::shared_ptr<Vec2>& translation);

    std::shared_ptr<Mat> rotate(float degrees);

    std::shared_ptr<Mat> rotateRad(float radians);

    std::shared_ptr<Mat> scale(float scaleX, float scaleY);

    std::shared_ptr<Mat> scale(const std::shared_ptr<Vec2>& scale);

    [[nodiscard]] std::vector<float> getValues() const;

    std::shared_ptr<Vec2> getTranslation(const std::shared_ptr<Vec2>& position);

    std::shared_ptr<Vec2> getScale(const std::shared_ptr<Vec2>& scale);

    float getRotation();

    float getRotationRad();

    std::shared_ptr<Mat> scl(float scale);

    std::shared_ptr<Mat> scl(const std::shared_ptr<Vec2>& scale);

    std::shared_ptr<Mat> scl(const std::shared_ptr<Vec3>& scale);

    std::shared_ptr<Mat> transpose();
};


#endif //MINDUSTRY_SRC_ARC_MATH_MAT_H
