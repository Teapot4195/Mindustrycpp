//
// Created by teapot on 22/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_MATHF_H
#define MINDUSTRY_SRC_ARC_MATH_MATHF_H

#include <memory>
#include <cmath>

#include <math/Rand.h>

// Forward decl to avoid possible future pains
class Vec2;
class Position;

class Mathf final {
public:
    static Rand rand;

    static float sin(float radians);

    static float cos(float radians);

    static float sinDeg(float degrees);

    static float cosDeg(float degrees);

    static float absin(float scl, float mag);

    static float absin(float in, float scl, float mag);

    static float tan(float radians, float scl, float mag);

    static float sin(float scl, float mag);

    static float sin(float radians, float scl, float mag);

    static float cos(float radians, float scl, float mag);

    static float angle(float x, float y);

    static float angleExact(float x, float y);

    static float wrapAngleAroundZero(float a);

private:
    static float atn(double i);

public:
    static float atan2(float x, float y);

    static int digits(int n);

    static int digits(long long n);

    static float sqrt(float x);

    static float sqr(float x);

    static float map(float value, float froma, float toa, float fromb, float tob);

    static float map(float value, float from, float to);

    static int sign(float f);

    static int sign(bool b);

    static int num(bool b);

    static float pow(float a, float b);

    static int pow(int a, int b);

    static float range(float range);

    static int range(int range);

    static float range(float min, float max);

    static bool chanceDelta(double d);

    static bool chance(double d);

    static int random(int range);

    static int random(int start, int end);

    static long long random(long long range);

    static long long random(long long start, long long end);

    static bool randomBoolean();

    static bool randomBoolean(float chance);

    static float random();

    static float random(float range);

    static float random(float start, float end);

    static int randomSign();

    static int randomSeed(long long seed, int min, int max);

    static float randomSeed(long long seed, float min, float max);

    static float randomSeed(long long seed);

    static float randomSeed(long long seed, float max);

    static float randomSeedRange(long long seed, float range);

    static float randomTriangular();

    static float randomTriangular(float max);

    static float randomTriangular(float min, float max);

    static float randomTriangular(float min, float max, float mode);

    static int nextPowerOfTwo(int value);

    static bool isPowerOfTwo(int value);

    static int clamp(int value, int min, int max);

    static long long clamp(long long value, long long min, long long max);

    static float clamp(float value, float min, float max);

    static float clamp(float value);

    static double clamp(double value, double min, double max);

    static float maxZero(float val);

    static float approach(float from, float to,  float speed);

    static float approachDelta(float from, float to, float speed);

    static float lerp(float fromValue, float toValue, float progress);

    static float lerpDelta(float fromValue, float toValue, float progress);

    static float slerpRad(float fromRadians, float toRadians, float progress);

    static float slerp(float fromDegrees, float toDegrees, float progress);

    static float slerpDelta(float fromDegrees, float toDegrees, float progress);

    static int floor(float value);

    static int floorPositive(float value);

    static int ceil(float value);

    static int ceilPositive(float value);

    static int round(float value);

    static int round(int value, int step);

    static float round(float value, float step);

    static int round(float value, int step);

    static int roundPositive(float value);

    static bool zero(float value);

    static bool zero(double value);

    static bool zero(float value, float tolerance);

    static bool equal(float a, float b);

    static bool equal(float a, float b, float tolerance);

    static float log(float a, float value);

    static float log2(float value);

    static int log2(int value);

    static float mod(float f, float n);

    static int mod(int x, int n);

    static float sample(std::vector<float> values, float time);

    static float slope(float fin);

    static float curve(float f, float offset);

    static float curve(float f, float from, float to);

    static float curveMargin(float f, float margin);

    static float curveMargin(float f, float marginLeft, float marginRight);

    static float len(float x, float y);

    static float len2(float x, float y);

    static float dot(float x1, float y1, float x2, float y2);

    static float dst(float x1, float y1);

    static float dst2(float x1, float y1);

    static float dst(float x1, float y1, float x2, float y2);

    static float dst2(float x1, float y1, float x2, float y2);

    static float dstm(float x1, float y1, float x2, float y2);

    static std::shared_ptr<Vec2> arrive(
            const std::shared_ptr<Position>& pos,
            const std::shared_ptr<Position>& target,
            const std::shared_ptr<Vec2>& curVel,
            float radius,
            float tolerance,
            float speed,
            float smoothTime
    );

    static std::shared_ptr<Vec2> arrive(
            float x,
            float y,
            float destX,
            float destY,
            const std::shared_ptr<Vec2>& curVel,
            float radius,
            float tolerance,
            float speed,
            float accel
    );

    static bool within(float x1, float y1, float x2, float y2, float dst);

    static bool within(float x1, float y1, float dst);

    constexpr static const int signs[] = {-1, 1};
    constexpr static const int zeroOne[] = {0, 1};
    constexpr static const bool booleans[] = {true, false};
    constexpr static const float FLOAT_ROUNDING_ERROR = 0x000001f; //32bits
    constexpr static const float PI = 3.1415927f, pi = PI, halfPi = PI/2;
    constexpr static const float PI2 = PI * 2;
    constexpr static const float E = 2.7182818f;
    static const float sqrt2, sqrt3;
    constexpr static const float radiansToDegrees = 180.f / PI;
    constexpr static const float radDeg = radiansToDegrees;
    constexpr static const float degreesToRadians = PI / 180;
    constexpr static const float degRad = degreesToRadians;
    constexpr static const double doubleDegRad = 0.017453292519943295;
    constexpr static const double doubleRadDeg = 57.29577951308232;

private:
    constexpr static const int sinBits = 14;
    constexpr static const int sinMask = ~(-1 << sinBits);
    constexpr static const int sinCount = sinMask + 1;
    static float* sinTable;
    constexpr static const float radFull = PI * 2;
    constexpr static const float degFull = 360;
    constexpr static const float radToIndex = sinCount / radFull;
    constexpr static const float degToIndex = sinCount / degFull;
    constexpr static const int BIG_ENOUGH_INT = 16*1024;
    constexpr static const double BIG_ENOUGH_FLOOR = BIG_ENOUGH_INT;
    constexpr static const double CEIL = 0.9999999;
    constexpr static const double BIG_ENOUGH_ROUND = BIG_ENOUGH_INT + 0.5f;
    static Rand seedr;
    static const std::shared_ptr<Vec2> v1, v2, v3;
};


#endif //MINDUSTRY_SRC_ARC_MATH_MATHF_H
